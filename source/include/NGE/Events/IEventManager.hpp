/* 
 * File:   IEventManager.hpp
 * Author: tku
 *
 * Created on 7 września 2015, 11:18
 */

#ifndef IEVENTMANAGER_HPP
#define	IEVENTMANAGER_HPP

#include "IEventData.hpp"


namespace NGE {
	namespace Events {

		typedef std::function<void(IEventDataPtr) > EventListenerDelegate;

		/**
		 * This is the object which maintains the list of registered events and their listeners.
		 * 
		 * This is a many-to-many relationship, as both one listener can be configured to proccess
		 * multiple event types and of course multiple listeners can be regsitered to each event type.
		 * 
		 * The interface to this construct uses smart pointer wrapped objects, the purpose being to ensure
		 * that no object that the registry is referring to is destroyed before it is removed from the
		 * registry and to allow for the registry to be the only place where this is list is kept.
		 * 
		 * Simple tearing down the registry (e.g. destroying it) will automatically clean up all
		 * pointed-to objects (so long as there are no other references).
		 */
		class IEventManager {

			enum constants {
				INFINITE = 0xffffffff
			};

			explicit IEventManager(const std::string& name, bool setAsGlobal);
			virtual ~IEventManager();

			/**
			 * Registers a delegate function that will get called when the event type is triggered.
			 * @param eventDelegate Delegate function that will be called.
			 * @param type Event type.
			 * @return True if successfuly registered, otherwise false.
			 */
			virtual bool AddListener(const EventListenerDelegate& eventDelegate, const EventType& type) = 0;

			/**
			 * Removes a delegate/event type pairing from the internal tables.
			 * @param eventDelegate Delegate function to remove.
			 * @param type Event type.
			 * @return True if successfuly removed, false if the pairing was not found.
			 */
			virtual bool RemoveListener(const EventListenerDelegate& eventDelegate, const EventType& type) = 0;

			/**
			 * Execute event now. This bypasses the queue entirely and immediately calls all delegate
			 * functions registered for the event.
			 * @param event Event to be executed.
			 * @return True if event was executed successfuly, otherwise false.
			 */
			virtual bool TriggerEvent(const IEventDataPtr& event) const = 0;
		};
	}
}

#endif	/* IEVENTMANAGER_HPP */

