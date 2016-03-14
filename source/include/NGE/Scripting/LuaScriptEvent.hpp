/* 
 * File:   LuaScriptEvent.hpp
 * Author: tku
 *
 * Created on 14 March 2016, 01:33
 */

#ifndef LUASCRIPTEVENT_HPP
#define LUASCRIPTEVENT_HPP

#include "NGE/Events/IEventData.hpp"
#include "NGE/Events/BaseEventData.hpp"

namespace NGE {
    namespace Scripting {

        class LuaScriptEvent : public NGE::Events::BaseEventData {
          private:
            static const NGE::Events::EventType eventType;
            
          public:

            explicit LuaScriptEvent() { }

            const NGE::Events::EventType& GetEventType() const override {
                return eventType;
            }
            
            NGE::Events::IEventDataPtr Copy() const override {
                return NGE::Events::IEventDataPtr(new LuaScriptEvent());
            }

            const std::string GetName() const override {
                return "LuaScriptEvent";
            }

            
            static void RegisterScriptClass();
        };
    }
}

#endif /* LUASCRIPTEVENT_HPP */

