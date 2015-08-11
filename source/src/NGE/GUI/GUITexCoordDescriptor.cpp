#include "NGE/GUI/GUITexCoordDescriptor.hpp"
#include "NGE/Math/Vector2.hpp"
#include "NGE/Math/MathUtils.hpp"
#include "NGE/Tools/Logger.hpp"
#include <string>

using namespace NGE::GUI;

namespace NGE {
	namespace GUI {
		Math::vec2i textureGUIDimensions(256, 256);
	}
}

void GUITexCoordDescriptior::SetTextureWidth(int width) {
	textureGUIDimensions.x = Math::MathUtils::Clamp(width, 8, 1024);
}

void GUITexCoordDescriptior::SetTextureHeight(int height) {
	textureGUIDimensions.y = Math::MathUtils::Clamp(height, 8, 1024);
}

GUITexCoordDescriptior::GUITexCoordDescriptior(const WidgetTypes type) {
	texCoords.Set(0, 0, 1, 1);
	widgetType = type;
}

GUITexCoordDescriptior::GUITexCoordDescriptior(const GUITexCoordDescriptior& copy) {
	this->operator=(copy);
}

GUITexCoordDescriptior& GUITexCoordDescriptior::operator=(const GUITexCoordDescriptior& copy) {
	if (this != &copy) {
		texCoords = copy.texCoords;
		widgetType = copy.widgetType;
	}
	return* this;
}

void GUITexCoordDescriptior::SetType(WidgetTypes type) {
	switch (type) {
		case SLIDER:
		case BUTTON:
		case CHECK_BOX:
		case CHECK_BOX_MARK:
		case RADIO_BUTTON:
		case CHECK_RB_MARK:
			widgetType = type;
			break;
		default:
			widgetType = UNKNOWN;
	}
}

void GUITexCoordDescriptior::SetType(const std::string& type) {
	if (type.length() == 0) {
		log_error("NULL GUITexCoordDescriptor type");
		return;
	}

	if (type == "MATERIAL_SURFACE")
		widgetType = MATERIAL_SURFACE;
	else if (type == "CHECK_BOX_MARK")
		widgetType = CHECK_BOX_MARK;
	else if (type == "CHECK_RB_MARK")
		widgetType = CHECK_RB_MARK;
	else if (type == "RADIO_BUTTON")
		widgetType = RADIO_BUTTON;
	else if (type == "SEPARATOR")
		widgetType = SEPARATOR;
	else if (type == "CHECK_BOX")
		widgetType = CHECK_BOX;
	else if (type == "TEXT_AREA")
		widgetType = TEXT_AREA;
	else if (type == "BUTTON")
		widgetType = BUTTON;
	else if (type == "SLIDER")
		widgetType = SLIDER;
	else if (type == "PANEL")
		widgetType = PANEL;
	else if (type == "LABEL")
		widgetType = LABEL;
	else {
		log_error("Unknown GUITexCoordDescriptor type -> " + type);
		widgetType = UNKNOWN;
	}
}

WidgetTypes GUITexCoordDescriptior::GetType() {
	return widgetType;
}

int GUITexCoordDescriptior::GetTextureWidth() {
	return textureGUIDimensions.x;
}

int GUITexCoordDescriptior::GetTextureHeight() {
	return textureGUIDimensions.y;
}

void GUITexCoordDescriptior::SetTexCoords(float x, float y, float z, float w) {
	texCoords.Set(Math::MathUtils::Clamp(x, 0.0f, 1.0f),
			Math::MathUtils::Clamp(y, 0.0f, 1.0f),
			Math::MathUtils::Clamp(z, 0.0f, 1.0f),
			Math::MathUtils::Clamp(w, 0.0f, 1.0f));
}

void GUITexCoordDescriptior::SetTexCoords(const NGE::Math::vec4f& texCoords) {
	this->texCoords = texCoords;
}

const NGE::Math::vec4f& GUITexCoordDescriptior::GetTexCoords() {
	return texCoords;
}
