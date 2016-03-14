#include "NGE/Scripting/LuaScriptEvent.hpp"
#include "NGE/Scripting/LuaScriptManager.hpp"
#include "NGE/Tools/Logger.hpp"

using namespace NGE::Scripting;
namespace k = kaguya;

const NGE::Events::EventType LuaScriptEvent::eventType(0x0f4bf9b3);

void LuaScriptEvent::RegisterScriptClass() {
	LuaScriptManager& manager = LuaScriptManager::GetInstance();
	(*manager.GetLuaState().lock())["Event"].setClass(k::ClassMetatable<NGE::Scripting::LuaScriptEvent>()
			.addConstructor()
			);
}

void LuaScriptEvent::RegisterEventTypeWithScript(const std::string& key, NGE::Events::EventType type) {
	LuaScriptManager& manager = LuaScriptManager::GetInstance();
	k::LuaTable eventTypeTable = (*manager.GetLuaState().lock())["EventType"];

	nge_log_info("LuaScriptEvent --> Registering EventType with script: key: '" + key + "', type: '" + nge_to_string(type) + "'");

	if (eventTypeTable.isNilref()) {
		eventTypeTable = (*manager.GetLuaState().lock()).newTable();
	}
	
	eventTypeTable[key] = type;
	(*manager.GetLuaState().lock())["EventType"] = eventTypeTable;
}
