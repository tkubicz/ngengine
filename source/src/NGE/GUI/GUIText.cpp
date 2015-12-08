#include "NGE/GUI/GUIText.hpp"
#include "NGE/Tools/Logger.hpp"
#include "NGE/Media/MediaManager.hpp"
#include "NGE/Math/MathUtils.hpp"

using namespace NGE::GUI;

GUIText::GUIText(const std::string& text) {
	GUIText(text.c_str());
}

GUIText::GUIText(const char* text) {
	update = true;
	color.Set(1.0f, 1.0f, 1.0f, 1.0f);
	scales.Set(1.0f, 1.0f);

	glGenBuffers(1, &vertexBuffer);
	vertexSize = 0;

	SetString(text);
}

GUIText::~GUIText() {
	glDeleteBuffers(1, &vertexBuffer);
}

GUIText::GUIText(const GUIText& copy) {
	this->operator=(copy);
}

GUIText& GUIText::operator=(const GUIText& copy) {
	if (this != &copy) {
		scales = copy.scales;
		update = copy.update;
		color = copy.color;
		size = copy.size;
		text = copy.text;
	}
	return* this;
}

GUIText& GUIText::operator=(const std::string& text) {
	SetString(text);
	return* this;
}

GUIText& GUIText::operator=(const char* text) {
	SetString(text);
	return* this;
}

bool GUIText::LoadXMLSettings(const pugi::xml_node& node) {
	if (node.empty() || string(node.name()) != "Text") {
		nge_log_error("Need a Text node");
		return false;
	}

#ifdef NGE_USE_FREETYPE
	SetFont(Media::MediaManager::getInstance().getFontManager().GetFont(node.child("FreeTypeFont")));
#endif
	SetString(node.attribute("string").as_string());
	SetHeightScale(node.attribute("hScale").as_float());
	SetWidthScale(node.attribute("wScale").as_float());
	SetColor(node.child("Color").attribute("r").as_float(), node.child("Color").attribute("g").as_float(), node.child("Color").attribute("b").as_float(), 1.0);

	return true;
}

const std::string& GUIText::GetString() {
	return text;
}

void GUIText::SetString(const std::string& text) {
	if (text.length() > 0 && this->text != text) {
		this->text = text;
		update = true;
	}
}

void GUIText::SetString(const char* text) {
	if (text && this->text != std::string(text)) {
		this->text = text;
		update = true;
	}
}

void GUIText::Clear() {
	text.clear();
	update = false;
	size.Set(0, 0);
}

void GUIText::Print(int x, int y) {
	if (!text.length())
		return;

	if (position.x != x || position.y != y) {
		position.Set(x, y);
		update = true;
	}

	ComputeDimensions();

#ifdef NGE_USE_FREETYPE
	if (!font)
		return;

	//font->PrintString(text, Math::vec2f(float(x), float(y)), Math::vec2f(scales.x, scales.y), color);
	font->PrintString(vertexBuffer, vertexSize, color);
#endif
}

void GUIText::PrintCenteredX(int x, int y) {
	if (!text.length())
		return;

	ComputeDimensions();
	Print(x - size.x / 2, y);
}

void GUIText::PrintCenteredY(int x, int y) {
	if (!text.length())
		return;

	ComputeDimensions();
	Print(x, y - size.y / 2);
}

void GUIText::PrintCenteredXY(int x, int y) {
	if (!text.length())
		return;

	ComputeDimensions();
	Print(x - size.x / 2, y - size.y / 2);
}

void GUIText::ComputeDimensions() {
	if (NeedUpdating()) {
#ifdef NGE_USE_FREETYPE
		if (!font)
			return;

		if (text.size()) {
			size = font->GetStringDimensions(text, Math::vec2f(scales.x, scales.y));
			//size.x = int(float(size.x) * scales.x);
			//size.y = int(float(size.y) * scales.y);

			if (update) {
				Math::vec2f fPosition((float) position.x, (float) position.y);
				font->GenerateVertices(vertexBuffer, vertexSize, text, fPosition, scales);
			}
		}
		//size.y = int(float(font->GetAtlasHeight()) * scales.y);
		ForceUpdate(false);
#endif
	}
}

void GUIText::SetSize(int x, int y) {
	size.Set(x, y);
}

void GUIText::SetSize(const NGE::Math::vec2i& size) {
	this->size = size;
}

int GUIText::GetHeight() {
	return size.y;
}

int GUIText::GetWidth() {
	return size.x;
}

const NGE::Math::vec2i& GUIText::GetSize() {
	return size;
}

bool GUIText::NeedUpdating() {
	return update;
}

void GUIText::ForceUpdate(bool update) {
	this->update = update;
}

void GUIText::SetColor(float r, float g, float b, float a) {
	color.Set(Math::MathUtils::Clamp(r, 0.0f, 255.0f),
			Math::MathUtils::Clamp(g, 0.0f, 255.0f),
			Math::MathUtils::Clamp(b, 0.0f, 255.0f),
			Math::MathUtils::Clamp(a, 0.0f, 1.0f));

	color.x /= (color.x > 1.0) ? 255.0f : 1.0f;
	color.y /= (color.y > 1.0) ? 255.0f : 1.0f;
	color.z /= (color.z > 1.0) ? 255.0f : 1.0f;
}

void GUIText::SetHeightScale(float height) {
	scales.y = Math::MathUtils::Clamp(height, 0.1f, 20.0f);
}

void GUIText::SetWidthScale(float width) {
	scales.x = Math::MathUtils::Clamp(width, 0.1f, 20.0f);
}

void GUIText::SetScales(const NGE::Math::vec2f& scales) {
	SetHeightScale(scales.y);
	SetWidthScale(scales.x);
}

float GUIText::GetHeightScale() {
	return scales.y;
}

float GUIText::GetWidthScale() {
	return scales.x;
}

const NGE::Math::vec2f& GUIText::GetScales() {
	return scales;
}

const NGE::Math::vec4f& GUIText::GetColor() {
	return color;
}

#ifdef NGE_USE_FREETYPE

void GUIText::SetFont(NGE::Media::Fonts::FreeTypeFont* font) {
	this->font = font;
}

NGE::Media::Fonts::FreeTypeFont* GUIText::GetFont() {
	return font;
}
#endif