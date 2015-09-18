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

void LuaScriptProcess::Succeed() {
	NGE::Core::Process::Succeed();
}

void LuaScriptProcess::Fail() {
	NGE::Core::Process::Fail();
}

void LuaScriptProcess::Pause() {
	NGE::Core::Process::Pause();
}

void LuaScriptProcess::UnPause() {
	NGE::Core::Process::UnPause();
}

bool LuaScriptProcess::IsAlive() const {
	return NGE::Core::Process::IsAlive();
}

bool LuaScriptProcess::IsDead() const {
	return NGE::Core::Process::IsDead();
}

bool LuaScriptProcess::IsRemoved() const {
	return NGE::Core::Process::IsRemoved();
}

bool LuaScriptProcess::IsPaused() const {
	return NGE::Core::Process::IsPaused();
}