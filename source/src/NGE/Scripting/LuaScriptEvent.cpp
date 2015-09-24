#include "NGE/Scripting/LuaScriptEvent.hpp"
#include "NGE/Scripting/LuaScriptManager.hpp"
#include "NGE/Tools/Logger.hpp"
using namespace NGE::Scripting;

void LuaScriptEvent::RegisterEventTypeWithScript(const std::string key, NGE::Events::EventType type) {
	// Get or create EventType table
	LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
	std::weak_ptr<sel::State> luaState = scriptManager.GetLuaState();

	if ((*luaState.lock()).CheckNil("EventType")) {
		scriptManager.ExecuteString("EventType = {}");
	}

	// TODO: Error checking
	
	(*luaState.lock())["EventType"][key.c_str()] = static_cast<unsigned int>(type);
	nge_log_info("LuaScriptEvent --> New script event: " + nge_to_string(type) + " registered with key: " + key);
}

