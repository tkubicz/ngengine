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

namespace NGE {
	namespace Scripting {

		class LuaScriptEventListener {
		  public:

			static bool RegisterEventListener(const std::string& name, kaguya::LuaFunction function) {
				NGE::Events::EventDelegate scriptDelegate(name, function);
				NGE::Events::EventManager& manager = NGE::Events::EventManager::GetInstance();
			}
		};
	}
}

#endif /* LUASCRIPTEVENTLISTENER_HPP */

