#include "NGE/GUI/GUIEvent.hpp"
#include "NGE/GUI/GUIRectangle.hpp"

using namespace NGE::GUI;

GUIEvent::GUIEvent(GUIRectangle* element) {
	callbackString = element ? element->GetCallbackString() : "Uknown";
	eventSource = element;
}

GUIEvent::GUIEvent(const GUIEvent& copy) {
	this->operator=(copy);
}

GUIEvent& GUIEvent::operator=(const GUIEvent& copy) {
	if (this != &copy) {
		callbackString = copy.callbackString;
		eventSource = copy.eventSource;
	}
	return *this;
}

GUIRectangle* GUIEvent::GetEventSource() {
	return eventSource;
}

const std::string& GUIEvent::GetCallbackString() {
	return callbackString;
}
