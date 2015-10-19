/* 
 * File:   IEventManager.hpp
 * Author: tku
 *
 * Created on 7 września 2015, 11:18
 */

#ifndef IEVENTMANAGER_HPP
#define	IEVENTMANAGER_HPP

#include "NGE/Events/IEventData.hpp"
#include "NGE/Core/ConcurrentQueue.hpp"

namespace NGE {
	namespace Events {

		typedef std::function<void(IEventDataPtr) > EventListenerDelegate;
		typedef Core::ConcurrentQueue<IEventDataPtr> ThreadSafeEventQueue;

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
		  public:

			enum constants {
				INFINITE = 0xffffffff
			};

			explicit IEventManager(const std::string& name, bool setAsGlobal);
			virtual ~IEventManager();

			/**
			 * Registers a delegate function that will get called when the event type is triggered.
			 * @param eventDelegateId Identifier of the delegate. It is needed to identify the delegate.
			 * @param eventDelegate Delegate function that will be called.
			 * @param type Event type.
			 * @return True if successfuly registered, otherwise false.
			 */
			virtual bool AddListener(const std::string& eventDelelgateId, const EventListenerDelegate& eventDelegate, const EventType& type) = 0;

			/**
			 * Removes a delegate/event type pairing from the internal tables.
			 * @param eventDelegateId Identifier of the delegate. It is needed to identify the delegate.
			 * @param type Event type.
			 * @return True if successfuly removed, false if the pairing was not found.
			 */
			virtual bool RemoveListener(const std::string& eventDelelgateId, const EventType& type) = 0;

			/**
			 * Execute event now. This bypasses the queue entirely and immediately calls all delegate
			 * functions registered for the event.
			 * @param event Event to be executed.
			 * @return True if event was executed successfuly, otherwise false.
			 */
			virtual bool TriggerEvent(const IEventDataPtr& event) const = 0;

			/**
			 * Execute event. This uses the queue and will call the delegate function on the next call
			 * to Update(), assuming there's enough time.
			 * @param event Event to be executed.
			 * @return True if everything went ok, otherwise false.
			 */
			virtual bool QueueEvent(const IEventDataPtr& event) = 0;

			/**
			 * Execute event in thread-safe manner.
			 * @param event Event to be executed.
			 * @return True if everything went ok, otherwise false.
			 */
			virtual bool ThreadSafeQueueEvent(const IEventDataPtr& event) = 0;

			/**
			 * Find the next available instance of the named event type and remove it from the processing
			 * queue. This may be done up to the point that it is actively being processed. For example, is
			 * safe to happen during event processing itself.
			 * @param type Event type.
			 * @param allOfType If true, then all events of that type are cleard from the input queue.
			 * @return True if the event was found and removed, otherwise false.
			 */
			virtual bool AbortEvent(const EventType& type, bool allOfType = false) = 0;

			/**
			 * Allow for processing of any queued messages, optionally specify a processing time limit so
			 * that the event processing does not take too long. Note the danger of using this artificial
			 * limiter is that all messages may not in fact get processed.
			 * @param maxMillis Maximum time for processing events in milliseconds.
			 * @return True if all messages ready for processing were completed, false otherwise (e.g. timeout).
			 */
			virtual bool Update(unsigned long maxMillis = INFINITE) = 0;

			/**
			 * Getter for the main global event manager. This is the event manager that is used by the majority of
			 * the engine, though you are free to define own as long as you instantiate it with setAsGlobal set to false.
			 * It is not valid to have more that one global event manager.
			 * @return Pointer to global event manager.
			 */
			static IEventManager* Get();
		};
	}
}

#endif	/* IEVENTMANAGER_HPP */
