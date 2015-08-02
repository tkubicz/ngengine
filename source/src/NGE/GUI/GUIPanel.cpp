#include "NGE/GUI/GUIPanel.hpp"
#include "NGE/GUI/GUITypes.hpp"
#include "NGE/GUI/GUIButton.hpp"
#include "NGE/GUI/GUILabel.hpp"
#include "NGE/GUI/GUISeparator.hpp"
#include "NGE/GUI/GUITextBox.hpp"
#include "NGE/Math/MathUtils.hpp"
#include "NGE/Tools/Logger.hpp"
#include <stdlib.h>

using namespace NGE::GUI;

GUIPanel::GUIPanel(const char* callback) : GUIRectangle(callback), GUIClippedRectangle() {
	SetInterval(4, 4);
	widgetType = PANEL;
	layout = FREE_LAYOUT;
}

GUIPanel::~GUIPanel() {
	Clear();
}

bool GUIPanel::LoadXMLSettings(const pugi::xml_node& node) {
	if (node.empty() || std::string(node.name()) != "Panel") {
		log_error("Need a Panel node in the xml file");
		return false;
	}

	if (!node.attribute("visible").empty())
		visible = node.attribute("visible").as_bool();
	else
		visible = true;

	if (!GUIRectangle::LoadXMLSettings(node) || !GUIClippedRectangle::LoadXMLClippedRectangleInfo(node))
		return false;

	for (pugi::xml_node widget = node.first_child(); widget; widget = widget.next_sibling()) {
		if (std::string(widget.name()) == "Panel") {
			GUIPanel* panel = new GUIPanel();
			if (!panel->LoadXMLSettings(widget) || !AddWidget(panel))
				delete panel;
			else {
				panel->ComputeWindowBounds();
				panel->ComputeClippedBounds(panel->GetWindowBounds());
			}
			continue;
		}

		if (std::string(widget.name()) == "Button") {
			GUIButton* newButton = new GUIButton(widget.attribute("name").as_string());
			if (!newButton->LoadXMLSettings(widget) || !AddWidget(newButton))
				delete newButton;
			else {
				newButton->ComputeWindowBounds();
			}
			continue;
		}

		if (std::string(widget.name()) == "Label") {
			GUILabel* newLabel = new GUILabel();
			if (!newLabel->LoadXMLSettings(widget) || !AddWidget(newLabel))
				delete newLabel;
			else {
				newLabel->ComputeWindowBounds();
			}
			continue;
		}

		if (std::string(widget.name()) == "Separator") {
			GUISeparator* newSeparator = new GUISeparator();
			if (!newSeparator->LoadXMLSettings(widget) || !AddWidget(newSeparator))
				delete newSeparator;
			else {
				newSeparator->ComputeWindowBounds();
			}
		}

		if (std::string(widget.name()) == "TextBox") {
			GUITextBox* newTextBox = new GUITextBox();
			if (!newTextBox->LoadXMLSettings(widget) || !AddWidget(newTextBox))
				delete newTextBox;
			else {
				newTextBox->ComputeWindowBounds();
			}
		}
	}

	return true;
}

GUIRectangle* GUIPanel::GetWidgetByCallbackString(const char* name) {
	if (!name)
		return NULL;

	GUIRectangle *element = NULL, *tPanel = NULL;

	size_t t = 0;

	for (t = 0; t < elements.size(); ++t) {
		if (elements[t]->GetCallbackString() == name)
			return elements[t];
	}

	for (t = 0; t < elements.size(); ++t) {
		if (elements[t]->GetWidgetType() == PANEL) {
			if (element = ((GUIPanel*) elements[t])->GetWidgetByCallbackString(name))
				break;
		}

		// TODO: Fix that after creating GUITabbedPanel
		/*if (elements[t]->GetWidgetType() == WidgetTypes.TABBED_PANEL)
		{
			if (element = ((GUITabbedPanel*)elements[t])->GetLowerPanel()->GetWidgetByCallbackString(name))
				break;
		}*/
	}
	return element;
}

void GUIPanel::CheckMouseEvents(NGE::Events::MouseEvent& event) {
	// Fix that
	if (visible && active) {
		GUIRectangle::CheckMouseEvents(event);
		for (size_t t = 0; t < elements.size(); ++t) {
			if (elements[t]->IsActive())
				elements[t]->CheckMouseEvents(event);
		}
	}
}

void GUIPanel::CheckKeyboardEvents(NGE::Events::KeyboardEvent& event) {
	if (visible && active) {
		GUIRectangle::CheckKeyboardEvents(event);
		for (size_t t = 0; t < elements.size(); ++t) {
			if (elements[t]->IsActive())
				elements[t]->CheckKeyboardEvents(event);
		}
	}
}

void GUIPanel::Update(float dt) {
	if (visible) {
		for (size_t i = 0; i < elements.size(); ++i)
			elements[i]->Update(dt);
	}
}

void GUIPanel::Render() {
	// TODO: Remove Comments
	//if (!IsAttached() || !parent)
	//    return;

	// TODO: Add combo widgets
	//Widgets comboBoxes;
	//GUIComboBox* comboPointer;

	if (visible) {
		RenderClippedBounds();
		for (size_t i = 0; i < elements.size(); ++i)
			elements[i]->Render();
	}
}

void GUIPanel::CollectZWidgets(ZWidgets& container) {
	if (visible) {
		for (size_t i = 0; i < elements.size(); ++i) {
			if (elements[i]->GetWidgetType() != PANEL) {
				ZWidget zWidget(elements[i]);
				zWidget.SetDistance(float(elements[i]->GetZCoordinate()));
				container.push_back(zWidget);
			} else {
				((GUIPanel*) elements[i])->CollectZWidgets(container);
			}

			ZWidget zWidget(this);
			zWidget.SetDistance(float(GetZCoordinate()));
			container.push_back(zWidget);
		}
	}
}

void GUIPanel::Pack() {
	if (!update)
		return;

	size_t i = 0;

	for (i = 0; i < elements.size(); ++i)
		elements[i]->ForceUpdate(update);

	if (!i) {
		//CorrectPosition();
		ComputeWindowBounds();
		ComputeClippedBounds(windowBounds);
		return;
	}

	switch (layout) {
		case YAXIS_LAYOUT:
		case YAXIS_CEN_LAYOUT:
			PackYAxisLayout();
			break;

		case XAXIS_LAYOUT:
			PackXAxisLayout();
			break;

		default:
			PackFreeLayout();
	}

	for (i = 0; i < elements.size(); ++i)
		elements[i]->ForceUpdate(update);
}

void GUIPanel::ForceUpdate(bool update) {
	this->update = update;
	Pack();
}

void GUIPanel::PackYAxisLayout() {
	ComputeWindowBounds();

	std::vector<int> childrenHeights, childrenWidths;

	size_t i = 0;
	int height = 0,
			xOffset = 0,
			panelWidth = GetWidth();

	for (i = 0; i < elements.size(); ++i) {
		const Math::vec4i& childBounds = elements[i]->GetWindowBounds();
		childrenHeights.push_back(childBounds.w - childBounds.y + interval.y);
		childrenWidths.push_back(childBounds.z - childBounds.x);
		height += childrenHeights[i];
		panelWidth = childrenWidths[i] > panelWidth ? childrenWidths[i] : panelWidth;
	}

	dimensions.Set(panelWidth, height);
	GUIRectangle::ComputeWindowBounds();

	windowBounds.z += interval.x * 2;
	windowBounds.w += interval.y;

	update = false;
	height = interval.y;

	CorrectPosition();
	ComputeClippedBounds(windowBounds);

	for (i = 0; i < elements.size(); ++i) {
		xOffset = (layout == YAXIS_CEN_LAYOUT) * (panelWidth - childrenWidths[i]) / 2;

		elements[i]->SetAnchorPoint(CORNERLU);
		elements[i]->SetPosition(interval.x + xOffset, height);
		elements[i]->ComputeWindowBounds();
		height += childrenHeights[i];
	}
}

void GUIPanel::PackXAxisLayout() {
	ComputeWindowBounds();

	std::vector<int> childrenWidhts, childrenHeights;
	float offset = 0;
	size_t i = 0;
	int height = 0,
			width = 0,
			panelHeight = windowBounds.w - windowBounds.y;

	for (i = 0; i < elements.size(); ++i) {
		const Math::vec4i& childBounds = elements[i]->GetWindowBounds();
		childrenHeights.push_back(childBounds.w - childBounds.y);
		childrenWidhts.push_back(childBounds.z - childBounds.x + interval.x);

		width += childrenWidhts[i];
		height = childBounds.w - childBounds.y;
		panelHeight = height > panelHeight ? height : panelHeight;
	}

	dimensions.Set(width, panelHeight);
	GUIRectangle::ComputeWindowBounds();

	windowBounds.z += interval.x;
	windowBounds.w += interval.y * 2;

	update = false;
	width = interval.x;

	CorrectPosition();
	ComputeClippedBounds(windowBounds);

	for (i = 0; i < elements.size(); ++i) {
		offset = Math::MathUtils::Clamp(float(panelHeight - childrenHeights[i]) / 2.0f + interval.y, 0.0f, 1000.0f);
		elements[i]->SetAnchorPoint(CORNERLU);
		elements[i]->SetPosition(width, int(offset));
		elements[i]->ComputeWindowBounds();
		width += childrenWidhts[i];
	}
}

void GUIPanel::PackFreeLayout() {
	if (!update || !parent)
		return;

	Math::vec4i newBounds;
	newBounds.Set(GetWindowBounds());
	size_t i = 0;

	for (i = 0; i < elements.size(); ++i) {
		elements[i]->ForceUpdate(update);
		//const Math::vec4i& childBounds = elements[i]->GetWindowBounds();

		//newBounds.x = (childBounds.x - clipSize) < newBounds.x ? childBounds.x - clipSize : newBounds.x;
		//newBounds.y = (childBounds.y - clipSize) < newBounds.y ? childBounds.y - clipSize : newBounds.y;
		//newBounds.z = (childBounds.z - clipSize) < newBounds.z ? childBounds.z - clipSize : newBounds.z;
		//newBounds.w = (childBounds.w - clipSize) < newBounds.w ? childBounds.w - clipSize : newBounds.w;
	}

	windowBounds = newBounds;
	update = false;

	CorrectPosition();
	ComputeClippedBounds(windowBounds);

	for (i = 0; i < elements.size(); ++i)
		elements[i]->ComputeWindowBounds();
}

void GUIPanel::CorrectPosition() {
	if (!parent)
		return;

	const Math::vec4i& parentBounds = parent->GetWindowBounds();
	bool update = false;
	int temp = windowBounds.z - windowBounds.x;

	if (position.x < 0) {
		if (parentBounds.z - windowBounds.z < abs(position.x)) {
			windowBounds.z = parentBounds.z + int(position.x);
			windowBounds.x = windowBounds.z - temp;
			update = true;
		}
	}

	if (position.x > 1) {
		if (windowBounds.x < position.x + parentBounds.x) {
			windowBounds.x = parentBounds.x + int(position.x);
			windowBounds.z = windowBounds.x + temp;
			update = true;
		}
	}

	if (position.y < 0) {
		if (parentBounds.w - windowBounds.w < abs(position.y)) {
			temp = windowBounds.w - windowBounds.y;
			windowBounds.w = parentBounds.w + int(position.y);
			windowBounds.y = windowBounds.w - temp;
			update = true;
		}
	}

	if (update) {
		for (size_t i = 0; i < elements.size(); ++i) {
			elements[i]->ForceUpdate(true);
			elements[i]->ComputeWindowBounds();
		}
	}
}

bool GUIPanel::AddWidget(GUIRectangle* widget) {
	if (widget) {
		for (size_t i = 0; i < elements.size(); ++i) {
			if (elements[i]->GetCallbackString() == widget->GetCallbackString()) {
				log_error("Panel already has a child with callbackString = " + widget->GetCallbackString());
				return false;
			}
		}

		widget->SetParent(this);
		elements.push_back(widget);
		return true;
	}
	return false;
}

void GUIPanel::Clear() {
	parent = NULL;

	for (size_t i = 0; i < elements.size(); ++i)
		delete elements[i];

	elements.clear();
}

void GUIPanel::Notify(GUIRectangle* element) {
	if (!element)
		return;

	switch (element->GetWidgetType()) {
		case RADIO_BUTTON:
			// TODO: Add Radio button implementation
			break;
	}
}

int GUIPanel::GetWidgetCountByType(int type) {
	int counter = 0;
	for (size_t i = 0; i < elements.size(); ++i) {
		if (elements[i]->GetWidgetType() == type)
			counter++;
	}
	return counter;
}

int GUIPanel::GetTreeHeight() {
	int height = 1;

	for (size_t i = 0; i < elements.size(); ++i) {
		if (elements[i]->GetWidgetType() == PANEL)
			height += ((GUIPanel*) elements[i])->GetTreeHeight();
	}
	return height;
}

void GUIPanel::SetInterval(int width, int height) {
	interval.Set(Math::MathUtils::Clamp(width, 0, 2500), Math::MathUtils::Clamp(height, 0, 2500));
}

void GUIPanel::SetInterval(const NGE::Math::vec2i& dimensions) {
	SetInterval(dimensions.x, dimensions.y);
}

const NGE::Math::vec2i& GUIPanel::GetInterval() {
	return interval;
}

void GUIPanel::SetLayout(int layout) {
	switch (layout) {
		case GRID_LAYOUT:
		case YAXIS_CEN_LAYOUT:
		case YAXIS_LAYOUT:
		case XAXIS_LAYOUT:
			this->layout = layout;
			break;

		default:
			this->layout = FREE_LAYOUT;
	}
}

int GUIPanel::GetLayout() {
	return layout;
}

Widgets& GUIPanel::GetWidgets() {
	return elements;
}