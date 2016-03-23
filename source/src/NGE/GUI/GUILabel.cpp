#include "NGE/GUI/GUILabel.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"

using namespace NGE::GUI;

GUILabel::GUILabel(const char* labelString, const char* callbackString) : GUIAlphaElement(callbackString) {
    SetDimensions(40, 5);
    SetLabelString(labelString);
    widgetType = GUI::LABEL;
}

bool GUILabel::LoadXMLSettings(const pugi::xml_node& node) {
    if (string(node.name()) != "Label") {
        log_error("Need Label node");
        return false;
    }

    if (node.attribute("name").empty() && node.attribute("callbackString").empty()) {
        log_error("Need 'name' or 'callbackString' attribute");
        return false;
    }

    SetCallbackString(!node.attribute("name").empty() ? node.attribute("name").as_string() : node.attribute("callbackString").as_string());

    return GUIAlphaElement::LoadXMLSettings(node);
}

void GUILabel::Update(float dt) { }

void GUILabel::Render() {
    if (!parent || !visible)
        return;

    // TODO: Sprawdzić to, to prawdopodobnie bug.
    //if (label.NeedUpdating())
    //parent->ForceUpdate(true);

    Math::vec2i center = GetCenter();

    label.Print(windowBounds.x, windowBounds.y);
}

const NGE::Math::vec4i& GUILabel::GetWindowBounds() {
    if (parent && update) {
        label.ComputeDimensions();
        //SetDimensions(label.GetWidth(), label.GetHeight());
        GUIRectangle::ComputeWindowBounds();

        log_debug("{}", windowBounds)
    }

    return windowBounds;
}
