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
#include "NGE/Events/MouseEvent.hpp"
#include "LuaScriptManager.hpp"
#include "LuaScriptProcess.hpp"
#include "LuaScriptEvent.hpp"

namespace e = NGE::Events;

namespace NGE {
	namespace Scripting {

		class LuaScriptEventListener {
		  public:

			struct ScriptEventListener {
				kaguya::LuaTable object;
				kaguya::LuaFunction function;
				e::EventListenerDelegate eventListenerDelegate;

				ScriptEventListener(kaguya::LuaTable object, kaguya::LuaFunction function) :
				object(object), function(function) {
					eventListenerDelegate = ([&object, &function](e::IEventDataPtr event) {
						function(object, event);
					});
				}

				e::EventDelegate GetEventDelegate(const std::string& name) {
					return e::EventDelegate(name, eventListenerDelegate);
				}
			};

			static std::vector<ScriptEventListener> delegateVector;

			static void RegisterEventListener(NGE::Events::EventType eventType, kaguya::LuaFunction function) {

				//				std::function<void(e::IEventDataPtr) > func = [](e::IEventDataPtr eventPtr) {
				//					function(eventPtr);
				//				};

				LuaScriptEvent* event = new LuaScriptEvent();
				function(event);
			}

			static void RegisterEventListenerInClass(NGE::Events::EventType eventType, kaguya::LuaTable obj, kaguya::LuaFunction function) {
				log_debug("Registering event listener from script for eventType: '{}'", eventType);

				//				e::EventListenerDelegate eventListenerDelegate = ([&obj, &function](e::IEventDataPtr event) {
				//					function(obj, event);
				//				});
				//
				//				// Create wrapper.
				//				e::EventDelegate* delegate = new e::EventDelegate("test-delegate", eventListenerDelegate);

				//std::shared_ptr<e::MouseEvent> event = std::make_shared<e::MouseEvent>();
				//eventListenerDelegate(event);

				//				e::EventManager::GetInstance().AddListener(*delegate, eventType);

				//delegateVector.push_back(ScriptEventListener(obj, function));
				//e::EventManager::GetInstance().AddListener(delegateVector[0].GetEventDelegate("test-delegate"), eventType);
			}

			static void QueueEvent(kaguya::LuaTable table) {
				if (!table["cpp_object"].isNilref()) {
					std::shared_ptr<LuaScriptEvent> event = table["cpp_object"];
					e::EventManager::GetInstance().QueueEvent(event);
				} else {
					log_error("Expecting shared object");
				}
			}

			static void RegisterScriptClass() {
				LuaScriptManager& manager = LuaScriptManager::GetInstance();
				auto ptr = manager.GetLuaState().lock();
				(*ptr)["register_event_listener"] = kaguya::overload(&LuaScriptEventListener::RegisterEventListener,
						&LuaScriptEventListener::RegisterEventListenerInClass);

				(*ptr)["queue_event"] = &LuaScriptEventListener::QueueEvent;
			}
		};

		std::vector<LuaScriptEventListener::ScriptEventListener> LuaScriptEventListener::delegateVector;
	}
}

#endif /* LUASCRIPTEVENTLISTENER_HPP */

