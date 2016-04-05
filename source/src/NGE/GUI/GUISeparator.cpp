#include "NGE/GUI/GUISeparator.hpp"
#include "NGE/GUI/GUIClippedRectangle.hpp"
#include "NGE/Math/MathUtils.hpp"
#include "NGE/Math/Vector4.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"
#include "NGE/Media/MediaManager.hpp"

using namespace NGE::GUI;

GUISeparator::GUISeparator(Orientation orientation) : GUIRectangle() {
	SetOrientation(orientation);
	SetRatio(1.0f);
	widgetType = SEPARATOR;
	SetColor(255, 165, 0);

	glGenBuffers(1, &vertexBuffer);
}

void GUISeparator::SetRatio(float ratio) {
	this->ratio = Math::MathUtils::Clamp(ratio, 0.1f, 1.0f);
}

float GUISeparator::GetRatio() {
	return ratio;
}

void GUISeparator::SetOrientation(Orientation orientation) {
	this->orientation = orientation == HORIZONTAL ? HORIZONTAL : VERTICAL;
}

void GUISeparator::SetOrientation(const std::string& orientation) {
	if (orientation == "HORIZONTAL")
		this->orientation = HORIZONTAL;
	else
		this->orientation = VERTICAL;
}

Orientation GUISeparator::GetOrientation() {
	return orientation;
}

void GUISeparator::SetColor(float r, float g, float b) {
	color.Set(Math::MathUtils::Clamp(r, 0.0f, 255.0f),
			Math::MathUtils::Clamp(g, 0.0f, 255.0f),
			Math::MathUtils::Clamp(b, 0.0f, 255.0f));
	color.x /= (color.x > 1.0) ? 255.0f : 1.0f;
	color.y /= (color.y > 1.0) ? 255.0f : 1.0f;
	color.z /= (color.z > 1.0) ? 255.0f : 1.0f;
}

const NGE::Math::vec3f& GUISeparator::GetColor() {
	return color;
}

bool GUISeparator::LoadXMLSettings(const pugi::xml_node& node) {
	if (string(node.name()) != "Separator") {
		log_error("Need Separator node");
		return false;
	}

	SetRatio(node.attribute("ratio").as_float());
	shader = Media::MediaManager::GetInstance().getShaderManager().getProgram(node.child("Shader"));

	if (shader == NULL) {
		log_error("Could not load shader for separator");
		return false;
	}

	if (!node.child("Color").empty()) {
		if (!node.child("Color").attribute("r").empty() && !node.child("Color").attribute("g").empty() && !node.child("Color").attribute("b").empty()) {
			SetColor(node.child("Color").attribute("r").as_float(),
					node.child("Color").attribute("g").as_float(),
					node.child("Color").attribute("b").as_float());
		}
	}

	SetOrientation(node.attribute("orientation").as_string());

	return GUIRectangle::LoadXMLSettings(node);
}

void GUISeparator::Update(float dt) {
}

void GUISeparator::Render() {
	if (!parent || ((GUIClippedRectangle*) parent)->IsBackgroundColorOn() || !shader)
		return;

	shader->bindShader();
	shader->sendUniform("guiType", 0);
	shader->sendUniform("color", color);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer((GLint) 0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_LINES, 0, 2);
	glDisableVertexAttribArray(0);
}

void const GUISeparator::ComputeWindowBounds() {
	if (parent && update) {
		GUIRectangle::ComputeWindowBounds();

		vertices[0] = Math::vec4f((float) windowBounds.x, (float) windowBounds.y, 0.f, 0.f);
		vertices[1] = Math::vec4f((float) windowBounds.z, (orientation == VERTICAL) ? (float) windowBounds.w : (float) windowBounds.y, 0.f, 0.f);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof (float) * 2 * 4, &vertices[0], GL_STATIC_DRAW);
	}
}

const NGE::Math::vec4i& GUISeparator::GetWindowBounds() {
	ComputeWindowBounds();
	return windowBounds;
}
