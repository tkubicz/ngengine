#include "NGE/Scripting/LuaScriptProcess.hpp"
#include "NGE/Scripting/LuaScriptManager.hpp"
using namespace NGE::Scripting;

LuaScriptProcess::LuaScriptProcess() {

}

void LuaScriptProcess::OnInit() {
	Process::OnInit();
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
//	(*luaState.lock())["ScriptProcess"].SetClass<Process>(
//			"Success", &LuaScriptProcess::Success,
//			"Fail", &LuaScriptProcess::Fail,
//			"Pause", &LuaScriptProcess::Pause,
//			"UnPause", &LuaScriptProcess::UnPause,
//			"IsAlive", &LuaScriptProcess::IsAlive,
//			"IsDead", &LuaScriptProcess::IsDead,
//			"IsPaused", &LuaScriptProcess::IsPaused,
//			"AttachChild", &LuaScriptProcess::AttachChild);
}

