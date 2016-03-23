#include "NGE/GUI/GUIRectangle.hpp"
#include "NGE/GUI/GUITypes.hpp"
#include "NGE/Rendering/Renderer.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"
#include "NGE/Core/Core.hpp"

using namespace NGE::GUI;

GUIRectangle::GUIRectangle(const char* callback) {
    SetCallbackString(callback);
    SetDimensions(5, 5);
    SetPosition(0, 0);

    lastAction = 0;
    widgetType = UNKNOWN;
    anchor = CORNERLD;
    parent = NULL;
    z = 1;

    mouseOver = false;
    released = false;
    pressed = false;
    clicked = false;
    focused = false;
    visible = true;
    active = true;
    update = false;

    ComputeWindowBounds();
}

void GUIRectangle::SetCallbackString(const std::string& callback) {
    callbackString = callback;
}

void GUIRectangle::SetCallbackString(const char* callback) {
    if (callback)
        callbackString = callback;
}

const std::string& GUIRectangle::GetCallbackString() {
    return callbackString;
}

void GUIRectangle::SetParent(GUIRectangle* parent) {
    this->parent = parent;
    update = (parent != NULL);
}

GUIRectangle* GUIRectangle::GetParent() {
    return parent;
}

bool GUIRectangle::LoadXMLSettings(const pugi::xml_node& node) {
    if (!node) {
        log_error("Could not load xml settings file for '{}' gui element", callbackString);
        return false;
    }

    if (node.attribute("callbackString").empty() && node.attribute("name").empty()) {
        log_error("Need <callbackString> or <name> attribute, check '{}', line: '{}'", node.name(), node.offset_debug());
        return false;
    } else if (!node.attribute("callbackString").empty())
        SetCallbackString(node.attribute("callbackString").as_string());
    else if (!node.attribute("name").empty())
        SetCallbackString(node.attribute("name").as_string());

    if (!node.attribute("anchorPoint").empty())
        SetAnchorPoint(node.attribute("anchorPoint").as_string());

    if (!node.attribute("visible").empty())
        SetVisible(node.attribute("visible").as_bool());

    if (!node.attribute("active").empty())
        SetActive(node.attribute("active").as_bool());

    if (!node.attribute("anchorPoint").empty())
        SetAnchorPoint(node.attribute("anchorPoint").as_string());

    if (!node.child("Position").empty()) {
        if (!node.child("Position").attribute("x").empty())
            position.x = node.child("Position").attribute("x").as_int();

        if (!node.child("Position").attribute("y").empty())
            position.y = node.child("Position").attribute("y").as_int();
    }

    if (!node.child("Dimensions").empty()) {
        if (!node.child("Dimensions").attribute("width").empty())
            dimensions.x = node.child("Dimensions").attribute("width").as_int();

        if (!node.child("Dimensions").attribute("height").empty())
            dimensions.y = node.child("Dimensions").attribute("height").as_int();
    }

    SetPosition(position);
    SetDimensions(dimensions);

    return true;
}

void GUIRectangle::SetDimensions(int width, int height) {
    dimensions.Set(width, height);
    update = true;
}

void GUIRectangle::SetDimensions(const NGE::Math::vec2i& dimensions) {
    SetDimensions(dimensions.x, dimensions.y);
}

const NGE::Math::vec2i& GUIRectangle::GetDimensions() {
    return dimensions;
}

void GUIRectangle::SetPosition(int xPos, int yPos) {
    position.Set(xPos, yPos);
    update = true;
}

void GUIRectangle::SetPosition(const NGE::Math::vec2i& position) {
    SetPosition(position.x, position.y);
}

const NGE::Math::vec2i& GUIRectangle::GetPosition() {
    return position;
}

NGE::Math::vec2i GUIRectangle::GetCenter() {
    GetWindowBounds();
    return Math::vec2i((windowBounds.x + windowBounds.z) / 2, (windowBounds.y + windowBounds.w) / 2);
}

void GUIRectangle::SetActive(bool active) {
    this->active = active;
}

bool GUIRectangle::IsActive() {
    return active;
}

void GUIRectangle::SetAnchorPoint(const std::string& anchorArg) {
    //if (!anchorArg)
    //    return;

    if (anchorArg == "CENTER") {
        anchor = CENTER;
    } else if (anchorArg == "CORNERLU") {
        anchor = CORNERLU;
    } else if (anchorArg == "CORNERRU") {
        anchor = CORNERRU;
    } else if (anchorArg == "CORNERLD") {
        anchor = CORNERLD;
    } else if (anchorArg == "CORNERRD") {
        anchor = CORNERRD;
    } else {
        anchor = CENTER;
    }
}

void GUIRectangle::SetAnchorPoint(int anchorArg) {
    switch (anchorArg) {
        case CORNERLU:
        case CORNERRU:
        case CORNERLD:
        case CORNERRD:
            anchor = anchorArg;
            break;
        default:
            anchor = CENTER;
    }
    update = true;
}

int GUIRectangle::GetAnchorPoint() {
    return anchor;
}

void GUIRectangle::ForceUpdate(bool update) {
    this->update = update;
}

void GUIRectangle::SetVisible(bool visible) {
    this->visible = visible;
}

bool GUIRectangle::IsVisible() {
    return visible;
}

bool GUIRectangle::IsAttached() {
    return (parent != NULL);
}

int GUIRectangle::GetWidgetType() {
    return widgetType;
}

void GUIRectangle::CheckMouseEvents(NGE::Events::MouseEvent& event) {
    Events::GUIEventListener* eventsListener = GetEventsListener();

    clicked = false;
    mouseOver = (event.GetYInverse() >= windowBounds.y) && (event.GetYInverse() <= windowBounds.w) && (event.GetX() >= windowBounds.x) && (event.GetX() <= windowBounds.z);

    if (!mouseOver) {
        if (pressed) // && event.GetAction() == Events::RELEASED)
            pressed = false;

        if (event.GetAction() == Events::PRESSED || event.GetAction() == Events::RELEASED)
            focused = false;
        return;
    }

    if (pressed && event.GetAction() == Events::RELEASED) {
        clicked = true;
        pressed = false;
        focused = true;
    }

    if (event.GetAction() == Events::PRESSED) {
        pressed = true;
    }

    if (eventsListener && EventDetected()) {
        GUIEvent event = GUIEvent(this);
        eventsListener->ActionPerformed(event);
    }
}

void GUIRectangle::CheckKeyboardEvents(NGE::Events::KeyboardEvent& event) { }

GUITexCoordDescriptior* GUIRectangle::GetTexCoordsInfo(int type) {
    return !parent ? NULL : parent->GetTexCoordsInfo(type);
}

NGE::Events::GUIEventListener* GUIRectangle::GetEventsListener() {
    return !parent ? NULL : parent->GetEventsListener();
}

void GUIRectangle::EnableGUITexture() {
    if (parent)
        parent->EnableGUITexture();
}

void GUIRectangle::DisableGUITexture() {
    if (parent)
        parent->DisableGUITexture();
}

void GUIRectangle::SetZCoordinate(int z) {
    this->z = z;
}

int GUIRectangle::GetZCoordinate() {
    return z;
}

const void GUIRectangle::ComputeWindowBounds() {
    if (update) {
        if (parent) {
            const Math::vec4i& parentBounds = parent->GetWindowBounds();

            z = parent->GetZCoordinate() + 1;
            Math::vec2f newDimension, newPosition;

            switch (anchor) {
                case CORNERLD:
                    windowBounds.x = parentBounds.x + position.x;
                    windowBounds.y = parentBounds.y + position.y;
                    windowBounds.z = windowBounds.x + dimensions.x;
                    windowBounds.w = windowBounds.y + dimensions.y;
                    break;

                case CORNERLU:
                    windowBounds.x = parentBounds.x + position.x;
                    windowBounds.y = parentBounds.w - position.y - dimensions.y;
                    windowBounds.z = windowBounds.x + dimensions.x;
                    windowBounds.w = parentBounds.w - position.y;
                    break;

                case CORNERRD:
                    windowBounds.x = parentBounds.z - position.x - dimensions.x;
                    windowBounds.y = parentBounds.y + position.y;
                    windowBounds.z = windowBounds.x + dimensions.x;
                    windowBounds.w = windowBounds.y + dimensions.y;
                    break;

                case CORNERRU:
                    windowBounds.x = parentBounds.z - position.x - dimensions.x;
                    windowBounds.y = parentBounds.w - position.y - dimensions.y;
                    windowBounds.z = windowBounds.x + dimensions.x;
                    windowBounds.w = parentBounds.w - position.y;
                    break;
            }
        } else {
            windowBounds.x = position.x;
            windowBounds.y = position.y;
            windowBounds.z = position.x + dimensions.x;
            windowBounds.w = position.y + dimensions.y;
        }
    }
}

const NGE::Math::vec4i& GUIRectangle::GetWindowBounds() {
    ComputeWindowBounds();
    return windowBounds;
}

bool GUIRectangle::EventDetected() {
    return mouseOver || released || focused || pressed || clicked;
}

int GUIRectangle::GetWidth() {
    return windowBounds.z - windowBounds.x;
}

int GUIRectangle::GetHeight() {
    return windowBounds.w - windowBounds.y;
}

bool GUIRectangle::IsPressed() {
    return pressed;
}

bool GUIRectangle::IsMouseOver() {
    return mouseOver;
}

bool GUIRectangle::IsClicked() {
    return clicked;
}

bool GUIRectangle::IsFocused() {
    return focused;
}
