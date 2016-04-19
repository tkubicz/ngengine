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
namespace c = NGE::Core;

namespace NGE {
	namespace Scripting {

		class LuaScriptEventListener {
		  public:

			struct ScriptEventListener {
				kaguya::LuaTable obj;
				kaguya::LuaFunction fun;

				ScriptEventListener(kaguya::LuaTable object, kaguya::LuaFunction function) : obj(object), fun(function) { }

				ScriptEventListener(kaguya::LuaFunction function) : fun(function) { }

				~ScriptEventListener() { }

				void RunFunctionWithObject(e::IEventDataPtr event) {
					if (!obj.isNilref() && !fun.isNilref()) {
						fun(obj, event);
					}
				}

				void RunFunction(e::IEventDataPtr event) {
					if (!fun.isNilref()) {
						fun(event);
					}
				}
			};

			static std::vector<ScriptEventListener*> delegateVector;

			static void RegisterEventListener(NGE::Events::EventType eventType, kaguya::LuaFunction function) {
				log_debug("Registering event listener from script for eventType: '{:x}'", eventType);

				ScriptEventListener* scriptEventListener = new ScriptEventListener(function);
				delegateVector.push_back(scriptEventListener);

				e::EventDelegate delegate(fmt::format("script-function-delegate-{}", delegateVector.size() + 1), c::make_delegate(*scriptEventListener, &ScriptEventListener::RunFunction));
				e::EventManager::GetInstance().AddListener(delegate, eventType);
			}

			static void RegisterEventListenerInClass(NGE::Events::EventType eventType, kaguya::LuaTable obj, kaguya::LuaFunction function) {
				log_debug("Registering event listener from script for eventType: '{:x}'", eventType);

				ScriptEventListener* scriptEventListener = new ScriptEventListener(obj, function);
				delegateVector.push_back(scriptEventListener);

				e::EventDelegate delegate(fmt::format("script-object-delegate-{}", delegateVector.size() + 1), c::make_delegate(*scriptEventListener, &ScriptEventListener::RunFunctionWithObject));
				e::EventManager::GetInstance().AddListener(delegate, eventType);
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

		std::vector<LuaScriptEventListener::ScriptEventListener*> LuaScriptEventListener::delegateVector;
	}
}

#endif /* LUASCRIPTEVENTLISTENER_HPP */

