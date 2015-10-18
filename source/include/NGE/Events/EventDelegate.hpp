/* 
 * File:   EventDelegate.hpp
 * Author: tku
 *
 * Created on 19 październik 2015, 00:19
 */

#ifndef EVENTDELEGATE_HPP
#define	EVENTDELEGATE_HPP

#include "NGE/Events/IEventData.hpp"

namespace NGE {
    namespace Events {
        typedef std::function<void(IEventDataPtr) > EventListenerDelegate;

        class EventDelegate {
          private:
            std::string eventDelegateId;
            EventListenerDelegate eventListenerDelegate;

          public:
            EventDelegate() {}
            EventDelegate(std::string eventDelgateId, EventListenerDelegate eventDelegate);

            std::string GetEventDelegateId() const;

            void SetEventDelegateId(std::string eventDelegateId);

            EventListenerDelegate GetEventListenerDelegate() const;

            void SetEventListenerDelegate(EventListenerDelegate eventListenerDelegate);
        };
    }
}

#endif	/* EVENTDELEGATE_HPP */

