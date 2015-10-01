/* 
 * File:   LuaScriptEventListenerManager.hpp
 * Author: tku
 *
 * Created on 27 wrzesień 2015, 18:59
 */

#ifndef LUASCRIPTEVENTLISTENERMANAGER_HPP
#define	LUASCRIPTEVENTLISTENERMANAGER_HPP

#include <set>

#include "LuaScriptEventListener.hpp"

namespace NGE {
    namespace Scripting {

        class LuaScriptEventListenerManager {
          private:
            typedef std::set<LuaScriptEventListener*> ScriptEventListenerSet;
            ScriptEventListenerSet listeners;

          public:
            LuaScriptEventListenerManager();
            ~LuaScriptEventListenerManager();

            void AddListener(LuaScriptEventListener* listener);
            void DestroyListener(LuaScriptEventListener* listener);

        };
    }
}

#endif	/* LUASCRIPTEVENTLISTENERMANAGER_HPP */

