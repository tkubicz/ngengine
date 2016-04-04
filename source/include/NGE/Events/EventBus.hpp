/* 
 * File:   EventBus.hpp
 * Author: tku
 *
 * Created on 8 September 2015, 10:56
 */

#ifndef EVENTBUS_HPP
#define EVENTBUS_HPP

#include <map>
#include <list>
#include "NGE/Events/AbstractEventBus.hpp"

namespace NGE {
	namespace Events {

		class EventBus : public AbstractEventBus {

			enum constants {
				NUM_QUEUES = 2
			};

			typedef std::map<std::string, EventListenerDelegate> EventDelegateMap;
			typedef std::map<EventType, EventDelegateMap> EventListenerMap;
			typedef std::list<IEventDataPtr> EventQueue;

			EventListenerMap eventListenersMap;
			EventQueue queues[NUM_QUEUES];

			/** Index of actively processing queue. Events enqueue to the opposing queue. */
			int activeQueue;

			ThreadSafeEventQueue realtimeEventQueue;

		  public:
			explicit EventBus(const std::string& name, bool setAsGlobal = false);
			virtual ~EventBus();

			virtual bool AddListener(const EventDelegate& delegate, const EventType& type) override;
			virtual bool RemoveListener(const EventDelegate& delegate, const EventType& type) override;

			virtual bool TriggerEvent(const IEventDataPtr& event) const override;
			virtual bool QueueEvent(const IEventDataPtr& event) override;
			virtual bool ThreadSafeQueueEvent(const IEventDataPtr& event) override;
			virtual bool AbortEvent(const EventType& type, bool allOfType) override;

			virtual bool Update(unsigned long maxMillis) override;
		};
	}
}

#endif /* EVENTBUS_HPP */

