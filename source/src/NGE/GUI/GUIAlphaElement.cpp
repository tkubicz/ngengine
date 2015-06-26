#include "NGE/GUI/GUIAlphaElement.hpp"
#include "NGE/Math/MathUtils.hpp"

using namespace NGE::GUI;

GUIAlphaElement::GUIAlphaElement(const char* callback) : GUIRectangle(callback) {
	SetColor(225.0f, 225.0f, 225.0f);
	alphaFadeScale = 1.0f;
	minAlpha = 0.6f;
	color.w = minAlpha;
}

bool GUIAlphaElement::LoadXMLSettings(const pugi::xml_node& node) {
	if (!node)
		return false;

	SetAlphaFadeScale(node.attribute("alphaFadeScale").as_float());
	SetMinAlpha(node.attribute("minAlpha").as_float());
	label.LoadXMLSettings(node.child("Text"));
	SetColor(node.child("Color").attribute("r").as_float(), node.child("Color").attribute("g").as_float(), node.child("Color").attribute("b").as_float());

	return GUIRectangle::LoadXMLSettings(node);
}

void GUIAlphaElement::SetColor(const NGE::Math::vec3f& color) {
	SetColor(color.x, color.y, color.z);
}

void GUIAlphaElement::SetColor(float r, float g, float b) {
	color.Set(Math::MathUtils::Clamp(r, 0.0f, 255.0f),
			Math::MathUtils::Clamp(g, 0.0f, 255.0f),
			Math::MathUtils::Clamp(b, 0.0f, 255.0f),
			Math::MathUtils::Clamp(color.w, 0.0f, 1.0f));

	color.x /= (color.x > 1.0) ? 255.0f : 1.0f;
	color.y /= (color.y > 1.0) ? 255.0f : 1.0f;
	color.z /= (color.z > 1.0) ? 255.0f : 1.0f;
}

const NGE::Math::vec4f& GUIAlphaElement::GetColor() {
	return color;
}

void GUIAlphaElement::SetLabelString(const char* label) {
	if (!label)
		return;
	this->label = label;
}

void GUIAlphaElement::SetLabelString(const std::string& label) {
	this->label = label;
}

GUIText* GUIAlphaElement::GetLabel() {
	return &label;
}

const std::string& GUIAlphaElement::GetLabelString() {
	return label.GetString();
}

void GUIAlphaElement::SetAlpha(float alpha) {
	color.w = Math::MathUtils::Clamp(alpha, minAlpha, 1.0f);
}

float GUIAlphaElement::GetAlpha() {
	return color.w;
}

void GUIAlphaElement::SetAlphaFadeScale(float scale) {
	alphaFadeScale = Math::MathUtils::Clamp(scale, 0.0f, 10.0f);
}

float GUIAlphaElement::GetAlphaFadeScale() {
	return alphaFadeScale;
}

void GUIAlphaElement::SetMinAlpha(float minAlpha) {
	this->minAlpha = Math::MathUtils::Clamp(minAlpha, 0.2f, 1.0f);
}

float GUIAlphaElement::GetMinAlpha() {
	return minAlpha;
}

void GUIAlphaElement::ModifyCurrentAlpha(float dt) {
	if (!mouseOver && !pressed)
		SetAlpha(color.w - dt * alphaFadeScale);
	else
		SetAlpha(color.w + dt * alphaFadeScale);
}