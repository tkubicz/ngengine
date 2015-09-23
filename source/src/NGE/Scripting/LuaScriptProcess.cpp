#include "NGE/Scripting/LuaScriptProcess.hpp"
#include "NGE/Scripting/LuaScriptManager.hpp"
#include "NGE/Scripting/LuaScriptEventListener.hpp"
using namespace NGE::Scripting;

LuaScriptProcess::LuaScriptProcess() {
	frequency = 10;
	time = 0;
}

void LuaScriptProcess::OnInit() {
}

void LuaScriptProcess::OnUpdate(unsigned int deltaTime) {
	time += deltaTime;
	if (time >= frequency) {
		if (scriptUpdateFunction) {
			scriptUpdateFunction(time);
		}
	}
}

void LuaScriptProcess::OnSuccess() {
	if (scriptSuccessFunction) {
		scriptSuccessFunction();
	}
}

void LuaScriptProcess::OnFail() {

}

void LuaScriptProcess::OnAbort() {
}

bool LuaScriptProcess::BuildProcessFromScript(const std::string& name) {
	LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
	std::weak_ptr<sel::State> luaState = scriptManager.GetLuaState();

	scriptInitFunction = (*luaState.lock())["process"]["OnInit"];
	scriptUpdateFunction = (*luaState.lock())["process"]["OnUpdate"];
	scriptSuccessFunction = (std::function<void() >) (*luaState.lock())["process"]["OnSuccess"];

	return true;
}

void LuaScriptProcess::RegisterScriptClass() {
	LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
	std::weak_ptr<sel::State> luaState = scriptManager.GetLuaState();

	(*luaState.lock())["ScriptProcess"].SetClass<LuaScriptProcess>("Succeed", &NGE::Scripting::LuaScriptProcess::Succeed,
			"Fail", &NGE::Scripting::LuaScriptProcess::Fail,
			"Pause", &NGE::Scripting::LuaScriptProcess::Pause,
			"UnPause", &NGE::Scripting::LuaScriptProcess::UnPause,
			"IsAlive", &NGE::Scripting::LuaScriptProcess::IsAlive,
			"IsDead", &NGE::Scripting::LuaScriptProcess::IsDead,
			"IsRemoved", &NGE::Scripting::LuaScriptProcess::IsRemoved,
			"IsPaused", &NGE::Scripting::LuaScriptProcess::IsPaused,
			"OnInit", &NGE::Scripting::LuaScriptProcess::OnInit,
			"AttachChild", &NGE::Scripting::LuaScriptProcess::ScriptAttachChild);

	(*luaState.lock())["CreateProcess"] = &NGE::Scripting::LuaScriptProcess::CreateFromScript;
}

void LuaScriptProcess::CreateFromScript(void* self, const std::string name) {
	LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
	std::weak_ptr<sel::State> luaState = scriptManager.GetLuaState();
	
	//std::function<void()> fun = (*luaState.lock())[name.c_str()]["OnInit"];
	//fun();
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

unsigned long int LuaScriptProcess::GetFrequency() const {
	return frequency;
}

unsigned long int LuaScriptProcess::GetTime() const {
	return time;
}

