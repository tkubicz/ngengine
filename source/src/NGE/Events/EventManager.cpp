#include <chrono>
#include "NGE/Events/EventManager.hpp"
#include "NGE/Tools/Logger.hpp"
using namespace NGE::Events;

EventManager::EventManager(const std::string& name, bool setAsGlobal) : IEventManager(name, setAsGlobal) {
	activeQueue = 0;
}

EventManager::~EventManager() {
}

bool EventManager::AddListener(const std::string& eventDelelgateId, const EventListenerDelegate& eventDelegate, const EventType& type) {
	log_info("Events --> Attempting to add delegate function for event type: " + to_string(type));

	// This will find or create the entry.
	EventDelegateMap& eventListenerMap = eventListeners[type];

	bool success = false;

	auto findIt = eventListenerMap.find(eventDelelgateId);
	if (findIt == eventListenerMap.end()) {
		eventListenerMap.insert(std::make_pair(eventDelelgateId, eventDelegate));
		log_info("Events --> Successfully added delegate for event type: " + to_string(type));
		success = true;
	} else {
		log_info("Events --> Attempting to double-register a delegate");
		success = false;
	}

	return success;
}

bool EventManager::RemoveListener(const std::string& eventDelelgateId, const EventType& type) {
	log_info("Events --> Attempting to remove delegate function from event type: " + to_string(type));
	bool success = false;

	auto findIt = eventListeners.find(type);
	if (findIt != eventListeners.end()) {
		EventDelegateMap& listeners = findIt->second;
		auto findListenerIt = listeners.find(eventDelelgateId);
		if (findListenerIt != listeners.end()) {
			listeners.erase(findListenerIt);
			success = true;
			log_info("Events --> Successfully removed delegate function - delegateId: " + eventDelelgateId + " for event type: " + to_string(type));
		}
	}

	return success;
}

bool EventManager::TriggerEvent(const IEventDataPtr& event) const {
	log_info("Events --> Attempting to trigger event: " + to_string(event->GetName()));
	bool processed = false;

	auto findIt = eventListeners.find(event->GetEventType());
	if (findIt != eventListeners.end()) {
		const EventDelegateMap& listeners = findIt->second;
		for (auto it = listeners.begin(); it != listeners.end(); ++it) {
			EventListenerDelegate listener = it->second;
			log_info("Events --> Sending event " + to_string(event->GetName()) + " to delegate");
			// Call the delegate.
			listener(event);
			processed = true;
		}
	}

	return processed;
}

bool EventManager::QueueEvent(const IEventDataPtr& event) {
	if (activeQueue < 0 || activeQueue > NUM_QUEUES) {
		return false;
	}

	if (!event) {
		log_error("Events --> Invalid event in QueueEvent()");
		return false;
	}

	log_info("Events --> Attempting to queue event: " + to_string(event->GetName()));

	auto findIt = eventListeners.find(event->GetEventType());
	if (findIt != eventListeners.end()) {
		queues[activeQueue].push_back(event);
		log_info("Events --> Successfully queued event: " + to_string(event->GetName()));
		return true;
	} else {
		log_info("Events --> Skipping event sience there are no delegates registered to receive it: " + to_string(event->GetName()));
		return false;
	}
}

bool EventManager::ThreadSafeQueueEvent(const IEventDataPtr& event) {
	realtimeEventQueue.Push(event);
	return true;
}

bool EventManager::AbortEvent(const EventType& type, bool allOfType) {
	if (activeQueue < 0 || activeQueue > NUM_QUEUES) {
		return false;
	}

	bool success = false;
	auto findIt = eventListeners.find(type);

	if (findIt != eventListeners.end()) {
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

bool EventManager::Update(unsigned long maxMillis) {
	using namespace std::chrono;
	unsigned long currMs = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
	unsigned long maxMs = ((maxMillis == IEventManager::INFINITE) ? (IEventManager::INFINITE) : (currMs + maxMillis));

	// Handle events from other threads.
	IEventDataPtr realtimeEvent;
	while (realtimeEventQueue.TryPop(realtimeEvent)) {
		QueueEvent(realtimeEvent);

		currMs = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
		if (maxMillis != IEventManager::INFINITE) {
			if (currMs >= maxMs) {
				log_error("Events --> A realtime process is spamming the event manager");
			}
		}
	}

	// Swap active queues and clear the new queue after the swap
	int queueToProcess = activeQueue;
	activeQueue = (activeQueue + 1) % NUM_QUEUES;
	queues[activeQueue].clear();

	log_info("EventLoop --> Processing Event Queue " + to_string(queueToProcess) + "; " + to_string(queues[queueToProcess].size()) + " events to process");

	// Process the queue.
	while (!queues[queueToProcess].empty()) {
		// Pop the fron of the queue.
		IEventDataPtr event = queues[queueToProcess].front();
		queues[queueToProcess].pop_front();
		log_info("EventLoop --> Processing Event " + to_string(event->GetName()));

		const EventType& eventType = event->GetEventType();

		// Find all delegate functions registered for this event.
		auto findIt = eventListeners.find(eventType);
		if (findIt != eventListeners.end()) {
			const EventDelegateMap& eventListeners = findIt->second;
			log_info("EventLoop --> Found " + to_string(eventListeners.size()) + " delegates");

			// Call each listener.
			for (auto it = eventListeners.begin(); it != eventListeners.end(); ++it) {
				EventListenerDelegate listener = it->second;
				log_info("EventLoop --> Sending event " + to_string(event->GetName()) + " to delegate");
				listener(event);
			}
		}

		// Check to see if time ran out.
		currMs = ((maxMillis == IEventManager::INFINITE) ? (IEventManager::INFINITE) : (currMs + maxMillis));
		if (maxMillis != IEventManager::INFINITE && currMs >= maxMs) {
			log_info("EventLoop --> Aborting event processing - time ran out");
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
