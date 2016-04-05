#include <chrono>
#include "NGE/Events/EventBus.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"
using namespace NGE::Events;

EventBus::EventBus() {
	activeQueue = 0;
}

EventBus::~EventBus() {
}

bool EventBus::AddListener(const EventDelegate& delegate, const EventType& type) {
	log_info("Attempting to add delegate function for event type: '{}'", type);

	// This will find or create the entry.
	EventDelegateMap& eventDelegateMap = eventListenersMap[type];

	bool success = false;

	auto findIt = eventDelegateMap.find(delegate.GetEventDelegateId());
	if (findIt == eventDelegateMap.end()) {
		eventDelegateMap.insert(std::make_pair(delegate.GetEventDelegateId(), delegate.GetEventListenerDelegate()));
		log_info("Successfully added delegate for event type: '{}'", type);
		success = true;
	} else {
		log_warn("Attempting to double-register a delegate");
		success = false;
	}

	return success;
}

bool EventBus::RemoveListener(const EventDelegate& delegate, const EventType& type) {
	log_info("Attempting to remove delegate function from event type: '{}'", type);
	bool success = false;

	auto findIt = eventListenersMap.find(type);
	if (findIt != eventListenersMap.end()) {
		EventDelegateMap& eventDelegateMap = findIt->second;
		auto findListenerIt = eventDelegateMap.find(delegate.GetEventDelegateId());
		if (findListenerIt != eventDelegateMap.end()) {
			eventDelegateMap.erase(findListenerIt);
			success = true;
			log_info("Successfully removed delegate function - delegateId: '{}' for event type: '{}'", delegate.GetEventDelegateId(), type);
		}
	}

	return success;
}

std::map<std::string, EventType> EventBus::ListAllListeners() {
	std::map<std::string, EventType> listenerList;
	for (auto& kv : eventListenersMap) {
		for (auto& qr : kv.second) {
			listenerList.insert(std::pair<std::string, EventType>(qr.first, kv.first));
		}
	}
	return listenerList;
}

bool EventBus::TriggerEvent(const IEventDataPtr& event) const {
	log_info("Attempting to trigger event: '{}'", event->GetName());
	bool processed = false;

	auto findIt = eventListenersMap.find(event->GetEventType());
	if (findIt != eventListenersMap.end()) {
		const EventDelegateMap& eventDelegateMap = findIt->second;
		for (auto it = eventDelegateMap.begin(); it != eventDelegateMap.end(); ++it) {
			EventListenerDelegate listener = it->second;
			log_info("Sending event '{}' to delegate", event->GetName());
			// Call the delegate.
			listener(event);
			processed = true;
		}
	}

	return processed;
}

bool EventBus::QueueEvent(const IEventDataPtr& event) {
	if (activeQueue < 0 || activeQueue > NUM_QUEUES) {
		return false;
	}

	if (!event) {
		log_error("Invalid event");
		return false;
	}

	log_info("Attempting to queue event: '{}'", event->GetName());

	auto findIt = eventListenersMap.find(event->GetEventType());
	if (findIt != eventListenersMap.end()) {
		queues[activeQueue].push_back(event);
		log_info("Successfully queued event: '{}'", event->GetName());
		return true;
	} else {
		log_warn("Skipping event since there are no delegates registered to receive it: '{}'", event->GetName());
		return false;
	}
}

bool EventBus::ThreadSafeQueueEvent(const IEventDataPtr& event) {
	realtimeEventQueue.Push(event);
	return true;
}

bool EventBus::AbortEvent(const EventType& type, bool allOfType) {
	if (activeQueue < 0 || activeQueue > NUM_QUEUES) {
		return false;
	}

	bool success = false;
	auto findIt = eventListenersMap.find(type);

	if (findIt != eventListenersMap.end()) {
		EventQueue& eventQueue = queues[activeQueue];
		auto it = eventQueue.begin();
		while (it != eventQueue.end()) {
			// Remove an item from the queue will invalidate the iterator, so have it
			// point to the next member. All work inside this loop will be done using
			// thisIt.
			auto thisIt = it;
			++it;

			if ((*thisIt)->GetEventType() == type) {
				eventQueue.erase(thisIt);
				success = true;
				if (!allOfType) {
					break;
				}
			}
		}
	}

	return success;
}

bool EventBus::Update(unsigned long maxMillis) {
	using namespace std::chrono;
	unsigned long currMs = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
	unsigned long maxMs = ((maxMillis == AbstractEventBus::INFINITE) ? (AbstractEventBus::INFINITE) : (currMs + maxMillis));

	// Handle events from other threads.
	IEventDataPtr realtimeEvent;
	while (realtimeEventQueue.TryPop(realtimeEvent)) {
		QueueEvent(realtimeEvent);

		currMs = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
		if (maxMillis != AbstractEventBus::INFINITE) {
			if (currMs >= maxMs) {
				log_warn("A real-time process is spamming the event manager");
			}
		}
	}

	// Swap active queues and clear the new queue after the swap
	int queueToProcess = activeQueue;
	activeQueue = (activeQueue + 1) % NUM_QUEUES;
	queues[activeQueue].clear();

	log_trace("Processing Event Queue: '{}'. Events to process: '{}'", queueToProcess, queues[queueToProcess].size());

	// Process the queue.
	while (!queues[queueToProcess].empty()) {
		// Pop the fron of the queue.
		IEventDataPtr event = queues[queueToProcess].front();
		queues[queueToProcess].pop_front();
		log_info("Processing Event: '{}'", event->GetName());

		const EventType& eventType = event->GetEventType();

		// Find all delegate functions registered for this event.
		auto findIt = eventListenersMap.find(eventType);
		if (findIt != eventListenersMap.end()) {
			const EventDelegateMap& eventDelegateMap = findIt->second;
			log_info("EventLoop --> Found '{}' delegates", eventDelegateMap.size());

			// Call each listener.
			for (auto it = eventDelegateMap.begin(); it != eventDelegateMap.end(); ++it) {
				EventListenerDelegate listener = it->second;
				log_info("Sending event '{}' to delegate", event->GetName());
				listener(event);
			}
		}

		// Check to see if time ran out.
		currMs = ((maxMillis == AbstractEventBus::INFINITE) ? (AbstractEventBus::INFINITE) : (currMs + maxMillis));
		if (maxMillis != AbstractEventBus::INFINITE && currMs >= maxMs) {
			log_info("Aborting event processing - time ran out");
			break;
		}
	}

	// If we couldn't process all of the events, push the remaining events to the new active queue.
	// Note: To preserve sequencing, go back-to-front, inserting them at the head of the active queue.
	bool queueFlushed = queues[queueToProcess].empty();
	if (!queueFlushed) {
		while (!queues[queueToProcess].empty()) {
			IEventDataPtr event = queues[queueToProcess].back();
			queues[queueToProcess].pop_back();
			queues[activeQueue].push_front(event);
		}
	}

	return queueFlushed;
}
