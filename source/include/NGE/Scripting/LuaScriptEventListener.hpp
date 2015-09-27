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

namespace NGE {
    namespace Scripting {

        class LuaScriptEventListener {
          private:
            std::string eventDelegateId;
            NGE::Events::EventType eventType;
            std::function<void(NGE::Events::IEventDataPtr) > scriptCallbackFunction;

          public:
            explicit LuaScriptEventListener(const std::string& eventDelelgateId, const NGE::Events::EventType& eventType, const std::function<void() > scriptCallbackFunction);
            virtual ~LuaScriptEventListener();

            NGE::Events::EventListenerDelegate GetDelegate();

            void ScriptEventDelegate(NGE::Events::IEventDataPtr eventPtr);
        };
    }
}

#endif	/* LUASCRIPTEVENTLISTENER_HPP */

