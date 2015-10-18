#include "NGE/Events/EventDelegate.hpp"
using namespace NGE::Events;

EventDelegate::EventDelegate(std::string eventDelgateId, EventListenerDelegate eventDelegate) : eventDelgateId(eventDelgateId), eventListenerDelegate(eventDelegate) { }

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
