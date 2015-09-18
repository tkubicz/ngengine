#include "NGE/Scripting/LuaScriptProcess.hpp"
#include "NGE/Scripting/LuaScriptManager.hpp"
using namespace NGE::Scripting;

LuaScriptProcess::LuaScriptProcess() {

}

void LuaScriptProcess::OnInit() {
	NGE::Core::Process::OnInit();
}

void LuaScriptProcess::OnUpdate(unsigned int deltaTime) {

}

void LuaScriptProcess::OnSuccess() {

}

void LuaScriptProcess::OnFail() {

}

void LuaScriptProcess::OnAbort() {
}

void LuaScriptProcess::RegisterScriptClass() {
	LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
	std::weak_ptr<sel::State> luaState = scriptManager.GetLuaState();
	(*luaState.lock())["Process"].SetClass<LuaScriptProcess>("Succeed", &NGE::Scripting::LuaScriptProcess::Succeed,
			"Fail", &NGE::Scripting::LuaScriptProcess::Fail,
			"Pause", &NGE::Scripting::LuaScriptProcess::Pause,
			"UnPause", &NGE::Scripting::LuaScriptProcess::UnPause,
			"IsAlive", &NGE::Scripting::LuaScriptProcess::IsAlive,
			"IsDead", &NGE::Scripting::LuaScriptProcess::IsDead,
			"IsRemoved", &NGE::Scripting::LuaScriptProcess::IsRemoved,
			"IsPaused", &NGE::Scripting::LuaScriptProcess::IsPaused,
			"AttachChild", &NGE::Scripting::LuaScriptProcess::ScriptAttachChild);
}