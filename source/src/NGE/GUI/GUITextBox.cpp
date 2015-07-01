#include "NGE/GUI/GUITextBox.hpp"
#include "NGE/Tools/Logger.hpp"
#include "NGE/Math/MathUtils.hpp"
#include "NGE/Media/MediaManager.hpp"

using namespace NGE::GUI;

GUITextBox::GUITextBox(const char* callbackString, const char* fieldText) : GUIAlphaElement(callbackString) {
	SetBordersColor(77, 77, 77);
	SetDimensions(80, 18);
	SetPadding(2, 2);
	SetColor(128, 128, 128);

	label.SetString(fieldText);

	blinkerPosition = 2;
	blinkerTimer = 0.0f;
	blinkerOn = false;

	textStartIndex = 0;
	textEndIndex = 0;

	//keyEvent = NULL;

	glGenBuffers(1, &vertexBuffer);
}

GUITextBox::~GUITextBox() {
	glDeleteBuffers(1, &vertexBuffer);
}

bool GUITextBox::LoadXMLSettings(const pugi::xml_node& node) {
	if (std::string(node.name()) != "TextBox") {
		Tools::Logger::WriteErrorLog("Need TextBox node");
		return false;
	}

	if (!node.child("BordersColor").empty()) {
		if (!node.child("BordersColor").attribute("r").empty() && !node.child("BordersColor").attribute("g").empty() &&
				!node.child("BordersColor").attribute("b").empty()) {
			SetBordersColor(node.child("BordersColor").attribute("r").as_float(),
					node.child("BordersColor").attribute("g").as_float(),
					node.child("BordersColor").attribute("b").as_float());
		}
	}

	shader = Media::MediaManager::getInstance().getShaderManager().getProgram(node.child("Shader"));
	if (!shader) {
		Tools::Logger::WriteErrorLog("Could not load shader");
		return false;
	}

	return GUIAlphaElement::LoadXMLSettings(node);
}

void GUITextBox::SetBordersColor(const NGE::Math::vec3f& color) {
	SetBordersColor(color.x, color.y, color.z);
}

void GUITextBox::SetBordersColor(float r, float g, float b) {
	bordersColor.Set(Math::MathUtils::Clamp(r, 0.0f, 255.0f),
			Math::MathUtils::Clamp(g, 0.0f, 255.0f),
			Math::MathUtils::Clamp(b, 0.0f, 255.0f));
	bordersColor.x /= (bordersColor.x > 1.0) ? 255.0f : 1.0f;
	bordersColor.y /= (bordersColor.y > 1.0) ? 255.0f : 1.0f;
	bordersColor.z /= (bordersColor.z > 1.0) ? 255.0f : 1.0f;
}

const NGE::Math::vec3f& GUITextBox::GetBordersColor() {
	return bordersColor;
}

void GUITextBox::SetPadding(const NGE::Math::vec2i& padding) {
	SetPadding(padding.x, padding.y);
}

void GUITextBox::SetPadding(int x, int y) {
	padding.Set(Math::MathUtils::Clamp(x, 0, 25), Math::MathUtils::Clamp(y, 0, 25));
}

const NGE::Math::vec2i& GUITextBox::GetPadding() {
	return padding;
}

void GUITextBox::Update(float dt) {
	if (!parent || !visible)
		return;

	ModifyCurrentAlpha(dt);

	if (keyEvent.GetAction() == Events::PRESSED) {
		if (keyEvent.GetKeyId() == Events::KEY_RIGHT) {
			blinkerPosition += (int) (8.f * dt);
			log_info(to_string(800 * dt));
		}
		else if (keyEvent.GetKeyId() == Events::KEY_LEFT)
			blinkerPosition -= (int) (8.f * dt);
	}

	blinkerTimer += dt;
	blinkerTimer -= blinkerTimer > 1.0f ? 1.0f : 0.0f;
}

void GUITextBox::Render() {
	if (!parent || !visible)
		return;

	GetWindowBounds();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader->bindShader();
	shader->autoEnableVertexAttribArray();

	shader->sendUniform("guiType", 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer((GLint) 0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	shader->sendUniform("color", color);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	shader->sendUniform("color", color);
	glDrawArrays(GL_LINE_LOOP, 4, 4);

	if (blinkerOn && blinkerTimer > 0.5f) {
		shader->sendUniform("color", Math::vec4f(0, 0, 1, 1));
		glDrawArrays(GL_LINES, 8, 2);
	}

	shader->autoDisableVertexAttribArray();

	glDisable(GL_BLEND);

	label.Print(windowBounds.x + padding.x, windowBounds.y + padding.y);
}

void GUITextBox::CheckMouseEvents(NGE::Events::MouseEvent& event) {
	GUIRectangle::CheckMouseEvents(event);

	if (mouseOver && clicked)
		SetupBlinker(event.GetX());

	blinkerOn = focused ? blinkerOn : false;
}

void GUITextBox::CheckKeyboardEvents(NGE::Events::KeyboardEvent& event) {
	if (!focused || !active)
		return;

	keyEvent = event;
}

void GUITextBox::SetupText(int type, char* info) { }

void GUITextBox::SetupBlinker(int mouseX) {
	if (!focused || !active)
		return;

	blinkerPosition = GetWindowBounds().x + padding.x;
	mouseX -= 1;

	const std::string& text = label.GetString();
#ifdef NGE_USE_FREETYPE
	const std::map<char, std::pair<int, int> > glyphDimensions = label.GetFont()->GetGlyphDimensions();
	const std::map<char, std::pair<int, int> > glyphAdvances = label.GetFont()->GetGlyphAdvances();
#endif

	//blinkerPosition += glyphDimensions.find('T')->second.first;

	for (size_t i = 0; i < text.length(); ++i) {
#ifdef NGE_USE_FREETYPE
		if (blinkerPosition < mouseX)
			blinkerPosition += glyphDimensions.find(int(text[i]))->second.first + glyphAdvances.find(int(text[i]))->second.first;
#endif
	}

	blinkerOn = true;
}

const NGE::Math::vec4i& GUITextBox::GetWindowBounds() {
	if (parent && visible) {
		label.ComputeDimensions();
		dimensions.y = label.GetHeight() ? label.GetHeight() + padding.y : dimensions.y;

		GUIRectangle::ComputeWindowBounds();
		blinkerPosition = !blinkerPosition ? windowBounds.x + padding.x : blinkerPosition;
		//blinkerPosition = windowBounds.x + padding.x;

		//SetupText(0, "0");

		Math::vec2f vertices[] = {
			Math::vec2f((float) windowBounds.x, (float) windowBounds.y),
			Math::vec2f((float) windowBounds.x, (float) windowBounds.w),
			Math::vec2f((float) (windowBounds.z + padding.x), (float) windowBounds.y),
			Math::vec2f((float) (windowBounds.z + padding.x), (float) windowBounds.w),

			Math::vec2f((float) windowBounds.x, (float) windowBounds.y),
			Math::vec2f((float) windowBounds.x, (float) windowBounds.w),
			Math::vec2f((float) (windowBounds.z + padding.x), (float) windowBounds.w),
			Math::vec2f((float) (windowBounds.z + padding.x), (float) windowBounds.y),

			Math::vec2f((float) blinkerPosition, (float) (windowBounds.y + padding.y) / 2.0f),
			Math::vec2f((float) blinkerPosition, (float) (windowBounds.w - padding.y))
		};

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof (float) * 2 * 10, &vertices[0], GL_DYNAMIC_DRAW);

		//vertices[8] = Math::vec2f(blinkerPosition + 50, windowBounds.y + padding.y / 2);
		//vertices[9] = Math::vec2f(blinkerPosition + 50, windowBounds.w - padding.y);

		//glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 8, sizeof(float) * 2 * 2, &vertices[8]);
	}
	return windowBounds;
}

