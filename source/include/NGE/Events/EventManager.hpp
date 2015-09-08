/* 
 * File:   EventManager.hpp
 * Author: tku
 *
 * Created on 8 września 2015, 10:56
 */

#ifndef EVENTMANAGER_HPP
#define	EVENTMANAGER_HPP

#include <map>
#include <list>
#include "NGE/Events/IEventManager.hpp"

namespace NGE {
	namespace Events {

		class EventManager : public IEventManager {

			enum constants {
				NUM_QUEUES = 2
			};

			typedef std::list<EventListenerDelegate> EventListenerList;
			typedef std::map<EventType, EventListenerList> EventListenerMap;
			typedef std::list<IEventDataPtr> EventQueue;

			EventListenerMap eventListeners;
			EventQueue queues[NUM_QUEUES];

			/** Index of actively processing queue. Events enque to the opossing queue. */
			int activeQueue;

			ThreadSafeEventQueue realtimeEventQueue;

		  public:
			explicit EventManager(const std::string& name, bool setAsGlobal);
			virtual ~EventManager();


			virtual bool AddListener(const EventListenerDelegate& eventDelegate, const EventType& type);
			virtual bool RemoveListener(const EventListenerDelegate& eventDelegate, const EventType& type);

			virtual bool TriggerEvent(const IEventDataPtr& event) const;
			virtual bool QueueEvent(const IEventDataPtr& event);
			virtual bool ThreadSafeQueueEvent(const IEventDataPtr& event);
			virtual bool AbortEvent(const EventType& type, bool allOfType);

			virtual bool Update(unsigned long maxMillis);
		};
	}
}

#endif	/* EVENTMANAGER_HPP */

