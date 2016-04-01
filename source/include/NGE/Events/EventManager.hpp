/* 
 * File:   EventManager.hpp
 * Author: tku
 *
 * Created on 1 April 2016, 02:48
 */

#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <list>
#include "NGE/Core/Singleton.hpp"

namespace NGE {
    namespace Events {

        typedef std::shared_ptr<E
        
        class EventManager : public NGE::Core::Singleton<EventManager> {
            friend class NGE::Core::Singleton<EventManager>;

          private:

            EventManager() { }

        };
    }
}

#endif /* EVENTMANAGER_HPP */

