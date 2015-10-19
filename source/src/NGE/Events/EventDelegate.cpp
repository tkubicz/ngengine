#include "NGE/Events/EventDelegate.hpp"
using namespace NGE::Events;

EventDelegate::EventDelegate(const std::string& eventDelgateId, EventListenerDelegate eventListenerDelegate) {
	this->eventDelegateId = eventDelgateId;
	this->eventListenerDelegate = eventListenerDelegate;
}

std::string EventDelegate::GetEventDelegateId() const {
	return eventDelegateId;
}

void EventDelegate::SetEventDelegateId(std::string eventDelegateId) {
	this->eventDelegateId = eventDelegateId;
}

EventListenerDelegate EventDelegate::GetEventListenerDelegate() const {
	return eventListenerDelegate;
}

void EventDelegate::SetEventListenerDelegate(EventListenerDelegate eventListenerDelegate) {
	this->eventListenerDelegate = eventListenerDelegate;
}
