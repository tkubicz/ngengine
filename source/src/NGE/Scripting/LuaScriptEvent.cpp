#include "NGE/Scripting/LuaScriptEvent.hpp"
#include "NGE/Scripting/LuaScriptManager.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"

using namespace NGE::Scripting;
namespace k = kaguya;

constexpr const NGE::Events::EventType LuaScriptEvent::eventType;

void LuaScriptEvent::RegisterScriptClass() {
	LuaScriptManager& manager = LuaScriptManager::GetInstance();
	(*manager.GetLuaState().lock())["Event"].setClass(k::ClassMetatable<NGE::Scripting::LuaScriptEvent>().addConstructor());
}

void LuaScriptEvent::RegisterEventTypeWithScript(const std::string& key, NGE::Events::EventType type) {
	LuaScriptManager& manager = LuaScriptManager::GetInstance();
	k::LuaTable eventTypeTable = (*manager.GetLuaState().lock())["EventType"];

	log_debug("Registering EventType with script: key: '{}', type: '{}'", key, type);

	if (eventTypeTable.isNilref()) {
		eventTypeTable = (*manager.GetLuaState().lock()).newTable();
	}

	eventTypeTable[key] = type;
	(*manager.GetLuaState().lock())["EventType"] = eventTypeTable;
}
