#include "NGE/Events/EventManager.hpp"
#include "NGE/Events/EventBus.hpp"
using namespace NGE::Events;

const char *EventManager::GLOBAL = "GLOBAL";

EventManager::EventManager() {
	CreateGlobalEventBus();
}

EventManager::~EventManager() {
	for (auto& bus : eventBusMap) {
		bus.second.reset();
	}
	eventBusMap.clear();
	globalEventBus.reset();
}

void EventManager::CreateGlobalEventBus() {
	std::shared_ptr<AbstractEventBus> globalEventBusInstance = std::make_shared<EventBus>();
	globalEventBus = globalEventBusInstance;
}

bool EventManager::FindEventBus(const std::string& name, EventBusMap::iterator& it) {
	auto find = eventBusMap.find(name);
	if (find == eventBusMap.end()) {
		log_warn("Could not find event bus: '{}'", name);
		return false;
	}

	it = find;
	return true;
}

EventBusPtr EventManager::operator[](const std::string& name) {
	if (name == GLOBAL) {
		return globalEventBus;
	}

	EventBusMapIter find;
	if (!FindEventBus(name, find)) {
		return false;
	}
	return find->second;
}

EventBusPtr EventManager::Create(const std::string& name) {
	EventBusMapIter find;
	if (FindEventBus(name, find)) {
		log_warn("Event bus: '{}' already exist. Returning existing instance", name);
		return find->second;
	}

	EventBusPtr eventBusPtr = std::make_shared<EventBus>();
	eventBusMap.insert(std::pair<std::string, EventBusPtr> (name, eventBusPtr));
	log_debug("Creating new event bus: '{}'", name);
	return eventBusPtr;
}

bool EventManager::Delete(const std::string& name) {
	EventBusMapIter find;
	if (!FindEventBus(name, find)) {
		return false;
	}
	eventBusMap.erase(find);
	log_debug("Deleting event bus: '{}'", name);
	return true;
}

bool EventManager::AddListener(const EventDelegate& delegate, const EventType& type) {
	return globalEventBus->AddListener(delegate, type);
}

bool EventManager::AddListener(const std::string& name, const EventDelegate& delegate, const EventType& type) {
	EventBusMapIter find;
	if (!FindEventBus(name, find)) {
		return false;
	}
	return find->second->AddListener(delegate, type);
}

bool EventManager::RemoveListener(const EventDelegate& delegate, const EventType& type) {
	return globalEventBus->RemoveListener(delegate, type);
}

bool EventManager::RemoveListener(const std::string& name, const EventDelegate& delegate, const EventType& type) {
	EventBusMapIter find;
	if (!FindEventBus(name, find)) {
		return false;
	}
	return find->second->RemoveListener(delegate, type);
}

bool EventManager::TriggerEvent(const IEventDataPtr& event) {
	return globalEventBus->TriggerEvent(event);
}

bool EventManager::TriggerEvent(const std::string& name, const IEventDataPtr& event) {
	EventBusMapIter find;
	if (!FindEventBus(name, find)) {
		return false;
	}
	return find->second->TriggerEvent(event);
}

bool EventManager::QueueEvent(const IEventDataPtr& event) {
	return globalEventBus->QueueEvent(event);
}

bool EventManager::QueueEvent(const std::string& name, const IEventDataPtr& event) {
	EventBusMapIter find;
	if (!FindEventBus(name, find)) {
		return false;
	}
	return find->second->QueueEvent(event);
}

bool EventManager::ThreadSafeEventQueue(const IEventDataPtr& event) {
	return globalEventBus->ThreadSafeQueueEvent(event);
}

bool EventManager::ThreadSafeEventQueue(const std::string& name, const IEventDataPtr& event) {
	EventBusMapIter find;
	if (!FindEventBus(name, find)) {
		return false;
	}
	return find->second->ThreadSafeQueueEvent(event);
}

bool EventManager::AbortEvent(const EventType& type, bool allOfType) {
	return globalEventBus->AbortEvent(type, allOfType);
}

bool EventManager::AbortEvent(const std::string& name, const EventType& type, bool allOfType) {
	EventBusMapIter find;
	if (!FindEventBus(name, find)) {
		return false;
	}
	return find->second->AbortEvent(type, allOfType);
}

bool EventManager::Update(unsigned long maxMillis) {
	globalEventBus->Update(maxMillis);
	return true;
}

bool EventManager::Update(const std::string& name, unsigned long maxMillis) {
	EventBusMapIter find;
	if (!FindEventBus(name, find)) {
		return false;
	}
	find->second->Update(maxMillis);
	return true;
}

bool EventManager::UpdateAll(unsigned long maxMillis) {
	Update(maxMillis);
	for (auto& kv : eventBusMap) {
		kv.second->Update(maxMillis);
	}
	return true;
}

EventBusMap& EventManager::GetEventBusMap() {
	return eventBusMap;
}

