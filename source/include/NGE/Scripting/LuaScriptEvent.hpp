/* 
 * File:   LuaScriptEvent.hpp
 * Author: tku
 *
 * Created on 24 September 2015, 12:42
 */

#ifndef LUASCRIPTEVENT_HPP
#define	LUASCRIPTEVENT_HPP

#include <map>
#include "NGE/Events/BaseEventData.hpp"

namespace NGE {
    namespace Scripting {

        class LuaScriptEvent : public NGE::Events::BaseEventData {
          private:
            typedef std::function<LuaScriptEvent*() > CreateEventForScriptFunctionType;
            typedef std::map<NGE::Events::EventType, CreateEventForScriptFunctionType> CreationFunctions;
            static CreationFunctions creationFunctions;

            bool eventDataIsValid;

          protected:
            // LuaObject eventData

          public:

            LuaScriptEvent();

            /**
             * Method used to register an event type in the script engine. It creates table EventType
             * if doesn't exist and adds key into it. So newly registered event is accessible from script e.g.
             * EventType.key.
             * @param key Key in EventType Lua table.
             * @param type EventType (unsigned long int) to be registered.
             */
            static void RegisterEventTypeWithScript(const std::string key, NGE::Events::EventType type);

            /**
             * This method is called to map an event creation function pointer with the event type. This allows
             * an event to be created by only knowing its type. This is required to allow scripts to trigger
             * the instantiation and queueing of events.
             * @param type EventType
             * @param creationFunction Pointer to the function (std::function...)
             */
            static void AddCreationFunction(NGE::Events::EventType type, CreateEventForScriptFunctionType creationFunction);

            static LuaScriptEvent* CreateEventFromScript(NGE::Events::EventType type);

          protected:
            /**
             * This method must be overridden if you want to fire this event from C++ and have it received by the script.
             * If you only fire the event from the script side, this method will never be called. It's purpose is to
             * fill in the eventData member, which is then passed to the script callback function in the listener. This
             * is only called the first time GetEventData() is called. If the event is script-only, this function does not
             * need to be overridden.
             */
            virtual void BuildEventData();

            /**
             * This method must be overridden if you want to fire this event from script and have it received by C++. If
             * you only fire this event from script and have it received by the script, it doesn't matter since eventData
             * will just be passed straight through. Its purpose is to fill in any C++ member variables using the data in
             * eventData (which is valid at the time of the call). It is called when the event is fired from script.
             * Return false if the data is invalid in some way, which will keep the event from actually firing.
             * @return 
             */
            virtual bool BuildEventFromScript() {
                return true;
            }
        };
    }
}

#endif	/* LUASCRIPTEVENT_HPP */

