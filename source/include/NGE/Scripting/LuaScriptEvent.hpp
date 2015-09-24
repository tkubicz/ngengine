/* 
 * File:   LuaScriptEvent.hpp
 * Author: tku
 *
 * Created on 24 września 2015, 12:42
 */

#ifndef LUASCRIPTEVENT_HPP
#define	LUASCRIPTEVENT_HPP

#include <map>
#include "NGE/Events/BaseEventData.hpp"

namespace NGE {
	namespace Scripting {

		class LuaScriptEvent : public NGE::Events::BaseEventData {
		  private:
			typedef std::map<NGE::Events::EventType, std::function<LuaScriptEvent*() >> CreationFunctions;
			static CreationFunctions creationFunctions;

			bool eventDataIsValid;

		  protected:
			// LuaObject eventData

		  public:

			LuaScriptEvent() : eventDataIsValid(false) { }

			/**
			 * Method used to register an event type with the script.
			 * @param key Key in EventType Lua table.
			 * @param type EventType (unsigned long int) to be registered.
			 */
			static void RegisterEventTypeWithScript(const std::string key, NGE::Events::EventType type);
		};
	}
}

#endif	/* LUASCRIPTEVENT_HPP */

