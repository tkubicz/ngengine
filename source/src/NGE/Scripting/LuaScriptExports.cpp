#include "NGE/Scripting/LuaScriptExports.hpp"
#include "NGE/Scripting/LuaScriptEvent.hpp"
#include "NGE/Scripting/LuaScriptManager.hpp"
#include "NGE/Core/Delegate.hpp"
using namespace NGE::Scripting;

void LuaScriptExports::Register() {
	LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
	std::weak_ptr<sel::State> luaState = scriptManager.GetLuaState();

	std::function<void(const std::string, unsigned int) > registerEventTypeWithScriptFunc = &LuaScriptEvent::RegisterEventTypeWithScript;
	(*luaState.lock())["RegisterEventType"] = registerEventTypeWithScriptFunc;
}

