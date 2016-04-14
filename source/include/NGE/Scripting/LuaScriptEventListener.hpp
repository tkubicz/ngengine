/* 
 * File:   LuaScriptEventListener.hpp
 * Author: tku
 *
 * Created on 12 April 2016, 17:48
 */

#ifndef LUASCRIPTEVENTLISTENER_HPP
#define LUASCRIPTEVENTLISTENER_HPP

#include <kaguya/kaguya.hpp>
#include "NGE/Core/Delegate.hpp"
#include "NGE/Events/EventDelegate.hpp"
#include "NGE/Events/EventManager.hpp"
#include "LuaScriptManager.hpp"
#include "LuaScriptProcess.hpp"
#include "LuaScriptEvent.hpp"

namespace e = NGE::Events;

namespace NGE {
	namespace Scripting {

		class LuaScriptEventListener {
		  public:

			static void RegisterEventListener(NGE::Events::EventType eventType, kaguya::LuaFunction function) {

				//				std::function<void(e::IEventDataPtr) > func = [](e::IEventDataPtr eventPtr) {
				//					function(eventPtr);
				//				};

				LuaScriptEvent* event = new LuaScriptEvent();
				function(event);
			}

			static void RegisterEventListenerInClass(NGE::Events::EventType eventType, kaguya::LuaTable obj, kaguya::LuaFunction function) {
				LuaScriptEvent* event = new LuaScriptEvent();
				function(obj, event);
			}

			static void QueueEvent(kaguya::LuaTable table) {
				if (!table["cpp_object"].isNilref()) {
					std::shared_ptr<LuaScriptEvent> event = table["cpp_object"];
					e::EventManager::GetInstance().QueueEvent(event);
				} else {
					log_error("Expecting shared object");
				}
			}

			static void RegisterScriptEventListerClass() {
				LuaScriptManager& manager = LuaScriptManager::GetInstance();
				auto ptr = manager.GetLuaState().lock();
				(*ptr)["register_event_listener"] = kaguya::overload(&LuaScriptEventListener::RegisterEventListener,
						&LuaScriptEventListener::RegisterEventListenerInClass);

				(*ptr)["queue_event"] = &LuaScriptEventListener::QueueEvent;
			}
		};
	}
}

#endif /* LUASCRIPTEVENTLISTENER_HPP */

