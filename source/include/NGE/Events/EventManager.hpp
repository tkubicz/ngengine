/* 
 * File:   EventManager.hpp
 * Author: tku
 *
 * Created on 1 April 2016, 02:48
 */

#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <map>
#include <memory>
#include "NGE/Core/Singleton.hpp"
#include "NGE/Events/AbstractEventBus.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"

namespace NGE {
	namespace Events {

		typedef std::map<std::string, std::shared_ptr<AbstractEventBus>> EventBusMap;
		typedef std::map<std::string, std::shared_ptr<AbstractEventBus>>::iterator EventBusMapIter;
		typedef std::shared_ptr<AbstractEventBus> EventBusPtr;

		class EventManager : public NGE::Core::Singleton<EventManager> {
			friend class NGE::Core::Singleton<EventManager>;

		  private:
			EventBusMap eventBusMap;
			EventBusPtr globalEventBus;

		  public:
			static const char* GLOBAL;

		  private:
			EventManager();
			~EventManager();

			void CreateGlobalEventBus();
			bool FindEventBus(const std::string& name, EventBusMapIter& it);

		  public:

			EventBusPtr operator[](const std::string& name);

			EventBusPtr Create(const std::string& name);
			bool Delete(const std::string& name);

			bool AddListener(const EventDelegate& delegate, const EventType& type);
			bool AddListener(const std::string& name, const EventDelegate& delegate, const EventType& type);

			bool RemoveListener(const EventDelegate& delegate, const EventType& type);
			bool RemoveListener(const std::string& name, const EventDelegate& delegate, const EventType& type);

			bool TriggerEvent(const IEventDataPtr& event);
			bool TriggerEvent(const std::string& name, const IEventDataPtr& event);
			
			bool QueueEvent(const IEventDataPtr& event);
			bool QueueEvent(const std::string& name, const IEventDataPtr& event);

			bool ThreadSafeEventQueue(const IEventDataPtr& event);
			bool ThreadSafeEventQueue(const std::string& name, const IEventDataPtr& event);

			bool AbortEvent(const EventType& type, bool allOfType = false);
			bool AbortEvent(const std::string& name, const EventType& type, bool allOfType = false);

			bool Update(unsigned long maxMillis = AbstractEventBus::constants::INFINITE);
			bool Update(const std::string& name, unsigned long maxMillis = AbstractEventBus::constants::INFINITE);
			bool UpdateAll(unsigned long maxMillis = AbstractEventBus::constants::INFINITE);

			EventBusMap& GetEventBusMap();
		};
	}
}

#endif /* EVENTMANAGER_HPP */

