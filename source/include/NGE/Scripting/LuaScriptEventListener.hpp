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
			std::string eventDelegateId;
			NGE::Events::EventType eventType;
			std::function<void(NGE::Events::IEventDataPtr) > scriptCallbackFunction;

		  public:
			explicit LuaScriptEventListener(const std::string& eventDelelgateId, const NGE::Events::EventType& eventType, std::function<void(NGE::Events::IEventDataPtr) > scriptCallbackFunction);
			virtual ~LuaScriptEventListener();

			NGE::Events::EventListenerDelegate GetDelegate();
			std::string GetDelegateId() const;
			
			void ScriptEventDelegate(NGE::Events::IEventDataPtr eventPtr);
		};
	}
}

#endif	/* LUASCRIPTEVENTLISTENER_HPP */

