/* 
 * File:   LuaScriptEventListener.hpp
 * Author: tku
 *
 * Created on 17 września 2015, 15:41
 */

#ifndef LUASCRIPTEVENTLISTENER_HPP
#define	LUASCRIPTEVENTLISTENER_HPP

#include "NGE/Core/Delegate.hpp"
#include "NGE/Events/IEventData.hpp"
#include "NGE/Events/IEventManager.hpp"
#include "NGE/Events/EventDelegate.hpp"

namespace NGE {
    namespace Scripting {

        /**
         * This is a C++ listnere proxy for script event listeners. It pairs a single event
         * type with a Lua callback function. Note that this event can be defined in C++ or Lua.
         * It may also be sent from C++ or Lua.
         * 
         * The Lua callback function should take in table with the event data. The return value
         * is ignored.
         */
        class LuaScriptEventListener {
          private:
            NGE::Events::EventType eventType;
            NGE::Events::EventDelegate scriptDelegate;

          public:
            explicit LuaScriptEventListener(const NGE::Events::EventDelegate& scriptDelegate, const NGE::Events::EventType& eventType);
            virtual ~LuaScriptEventListener();

            NGE::Events::EventListenerDelegate GetDelegate();
            std::string GetDelegateId() const;

			NGE::Events::EventDelegate GetEventDelegate() const {
				return scriptDelegate;
			}
			
            void ScriptEventDelegate(NGE::Events::IEventDataPtr eventPtr);
        };
    }
}

#endif	/* LUASCRIPTEVENTLISTENER_HPP */

