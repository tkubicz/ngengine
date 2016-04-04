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

namespace NGE {
    namespace Events {

        class EventManager : public NGE::Core::Singleton<EventManager> {
            friend class NGE::Core::Singleton<EventManager>;

          private:
            std::map<std::string, std::shared_ptr<AbstractEventBus>> eventBusMap;
            std::shared_ptr<AbstractEventBus> globalEventBus;

          private:
            EventManager();
            ~EventManager();

          protected:
            void CreateGlobalEventBus();

          public:
            std::weak_ptr<AbstractEventBus> Create(const std::string& name);
            bool Delete(const std::string& name);

        };
    }
}

#endif /* EVENTMANAGER_HPP */

