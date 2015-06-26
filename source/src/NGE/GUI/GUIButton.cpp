#include "NGE/GUI/GUIButton.hpp"
#include "NGE/Tools/Logger.hpp"
#include "NGE/Rendering/Renderer.hpp"
#include "NGE/Media/MediaManager.hpp"

using namespace NGE::GUI;

GUIButton::GUIButton(const char* callback) : GUIAlphaElement(callback), GUIClippedRectangle() {
	SetBordersColor(0.0f, 0.0f, 0.0f);
	SetDimensions(40, 22);
	SetPosition(0, 0);
	SetColor(100, 150, 190);

	widgetType = BUTTON;

	drawBackground = true;
	drawBounds = true;
	bounce = true;
}

bool GUIButton::LoadXMLSettings(const pugi::xml_node& node) {
	if (std::string(node.name()) != "Button") {
		Tools::Logger::WriteErrorLog("Need a Button node");
		return false;
	}

	bounce = node.attribute("bounce").as_bool();

	return GUIAlphaElement::LoadXMLSettings(node) && GUIClippedRectangle::LoadXMLClippedRectangleInfo(node);
}

void GUIButton::EnableBounce(bool bounce) {
	this->bounce = bounce;
}

bool GUIButton::BounceEnabled() {
	return bounce;
}

void GUIButton::Update(float dt) {
	ModifyCurrentAlpha(dt);
}

void GUIButton::Render() {
	if (!parent || !visible)
		return;

	backgroundColor.w = color.w;

	float displacement = 2.0f * (pressed) * bounce;

	int xCenter = (windowBounds.x + windowBounds.z) / 2;
	int yCenter = (windowBounds.y + windowBounds.w) / 2;

	//Rendering::Renderer::GetMatrixStack().PushMatrix();
	Rendering::Renderer::GetInstance().GetMatrixStack().Translate(displacement, displacement, 0.0);
	RenderClippedBounds();
	label.PrintCenteredXY(xCenter, yCenter);
	Rendering::Renderer::GetInstance().GetMatrixStack().Translate(-displacement, -displacement, 0.0);
	//Rendering::Renderer::GetMatrixStack().PopMatrix();
}

const void GUIButton::ComputeWindowBounds() {
	if (parent && update) {
		GUIRectangle::ComputeWindowBounds();
		label.ComputeDimensions();

		int width = windowBounds.z - windowBounds.x;
		int height = windowBounds.w - windowBounds.y;

		if (width <= label.GetWidth() + 2 * clipSize) {
			if (anchor == CENTER) {
				width = (label.GetWidth() - width) / 2 + clipSize + 2;
				windowBounds.x -= width;
				windowBounds.z += width;
			} else if ((anchor == CORNERLU) || (anchor == CORNERLD)) {
				width = (label.GetWidth() - width) / 2 + clipSize + 2;
				windowBounds.z += 2 * width;
			}
		}

		if (height + 2 * clipSize < label.GetHeight()) {
			height = (label.GetHeight() - height) / 2 + clipSize + 2;
			windowBounds.y -= height;
			windowBounds.w += height;
		}

		ComputeClippedBounds(windowBounds);
	}
}

