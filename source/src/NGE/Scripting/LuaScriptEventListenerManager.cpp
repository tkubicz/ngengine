#include "NGE/Scripting/LuaScriptEventListenerManager.hpp"
#include "NGE/Tools/Logger.hpp"
using namespace NGE::Scripting;

LuaScriptEventListenerManager::LuaScriptEventListenerManager() {
}

LuaScriptEventListenerManager::~LuaScriptEventListenerManager() {
	for (auto it = listeners.begin(); it != listeners.end(); ++it) {
		LuaScriptEventListener* listener = (*it);
		delete listener;
	}
	listeners.clear();
}

void LuaScriptEventListenerManager::AddListener(LuaScriptEventListener* listener) {
	listeners.insert(listener);
}

void LuaScriptEventListenerManager::DestroyListener(LuaScriptEventListener* listener) {
	ScriptEventListenerSet::iterator findIt = listeners.find(listener);
	if (findIt != listeners.end()) {
		listeners.erase(findIt);
		delete listener;
	} else {
		nge_log_error("LuaScriptEventListenerManager --> Could not find script listener in set. This will probably cause a memory leak");
	}
}

unsigned long int LuaScriptEventListenerManager::GetListenersSize() {
	return listeners.size();
}
