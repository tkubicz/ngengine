#include <chrono>
#include "NGE/Events/EventManager.hpp"
#include "NGE/Tools/Logger.hpp"
using namespace NGE::Events;

EventManager::EventManager(const std::string& name, bool setAsGlobal) : IEventManager(name, setAsGlobal) {
    activeQueue = 0;
}

EventManager::~EventManager() { }

bool EventManager::AddListener(const EventDelegate& delegate, const EventType& type) {
    nge_log_info("Events --> Attempting to add delegate function for event type: " + nge_to_string(type));

    // This will find or create the entry.
    EventDelegateMap& eventDelegateMap = eventListenersMap[type];

    bool success = false;

    auto findIt = eventDelegateMap.find(delegate.GetEventDelegateId());
    if (findIt == eventDelegateMap.end()) {
        eventDelegateMap.insert(std::make_pair(delegate.GetEventDelegateId(), delegate.GetEventListenerDelegate()));
        nge_log_info("Events --> Successfully added delegate for event type: " + nge_to_string(type));
        success = true;
    } else {
        nge_log_info("Events --> Attempting to double-register a delegate");
        success = false;
    }

    return success;
}

bool EventManager::RemoveListener(const EventDelegate& delegate, const EventType& type) {
    nge_log_info("Events --> Attempting to remove delegate function from event type: " + nge_to_string(type));
    bool success = false;

    auto findIt = eventListenersMap.find(type);
    if (findIt != eventListenersMap.end()) {
        EventDelegateMap& eventDelegateMap = findIt->second;
        auto findListenerIt = eventDelegateMap.find(delegate.GetEventDelegateId());
        if (findListenerIt != eventDelegateMap.end()) {
            eventDelegateMap.erase(findListenerIt);
            success = true;
            nge_log_info("Events --> Successfully removed delegate function - delegateId: " + delegate.GetEventDelegateId() + " for event type: " + nge_to_string(type));
        }
    }

    return success;
}

bool EventManager::TriggerEvent(const IEventDataPtr& event) const {
    nge_log_info("Events --> Attempting to trigger event: " + nge_to_string(event->GetName()));
    bool processed = false;

    auto findIt = eventListenersMap.find(event->GetEventType());
    if (findIt != eventListenersMap.end()) {
        const EventDelegateMap& eventDelegateMap = findIt->second;
        for (auto it = eventDelegateMap.begin(); it != eventDelegateMap.end(); ++it) {
            EventListenerDelegate listener = it->second;
            nge_log_info("Events --> Sending event " + nge_to_string(event->GetName()) + " to delegate");
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
        nge_log_error("Events --> Invalid event in QueueEvent()");
        return false;
    }

    nge_log_info("Events --> Attempting to queue event: " + nge_to_string(event->GetName()));

    auto findIt = eventListenersMap.find(event->GetEventType());
    if (findIt != eventListenersMap.end()) {
        queues[activeQueue].push_back(event);
        nge_log_info("Events --> Successfully queued event: " + nge_to_string(event->GetName()));
        return true;
    } else {
        nge_log_info("Events --> Skipping event sience there are no delegates registered to receive it: " + nge_to_string(event->GetName()));
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
                nge_log_error("Events --> A realtime process is spamming the event manager");
            }
        }
    }

    // Swap active queues and clear the new queue after the swap
    int queueToProcess = activeQueue;
    activeQueue = (activeQueue + 1) % NUM_QUEUES;
    queues[activeQueue].clear();

    nge_log_info("EventLoop --> Processing Event Queue " + nge_to_string(queueToProcess) + "; " + nge_to_string(queues[queueToProcess].size()) + " events to process");

    // Process the queue.
    while (!queues[queueToProcess].empty()) {
        // Pop the fron of the queue.
        IEventDataPtr event = queues[queueToProcess].front();
        queues[queueToProcess].pop_front();
        nge_log_info("EventLoop --> Processing Event " + nge_to_string(event->GetName()));

        const EventType& eventType = event->GetEventType();

        // Find all delegate functions registered for this event.
        auto findIt = eventListenersMap.find(eventType);
        if (findIt != eventListenersMap.end()) {
            const EventDelegateMap& eventDelegateMap = findIt->second;
            nge_log_info("EventLoop --> Found " + nge_to_string(eventDelegateMap.size()) + " delegates");

            // Call each listener.
            for (auto it = eventDelegateMap.begin(); it != eventDelegateMap.end(); ++it) {
                EventListenerDelegate listener = it->second;
                nge_log_info("EventLoop --> Sending event " + nge_to_string(event->GetName()) + " to delegate");
                listener(event);
            }
        }

        // Check to see if time ran out.
        currMs = ((maxMillis == IEventManager::INFINITE) ? (IEventManager::INFINITE) : (currMs + maxMillis));
        if (maxMillis != IEventManager::INFINITE && currMs >= maxMs) {
            nge_log_info("EventLoop --> Aborting event processing - time ran out");
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
