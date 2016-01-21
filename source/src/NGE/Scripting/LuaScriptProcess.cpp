#include "NGE/Scripting/LuaScriptProcess.hpp"
#include "NGE/Scripting/LuaScriptManager.hpp"
using namespace NGE::Scripting;

void LuaScriptProcess::RegisterScriptClass() {
	LuaScriptManager& manager = LuaScriptManager::GetInstance();
	(*manager.GetLuaState().lock())["Process"].setClass(kaguya::ClassMetatable<NGE::Core::Process>()
			.addMember("succeed", &NGE::Core::Process::Succeed)
			.addMember("fail", &NGE::Core::Process::Fail)
			.addMember("pause", &NGE::Core::Process::Pause)
			.addMember("unpause", &NGE::Core::Process::UnPause)
			.addMember("is_alive", &NGE::Core::Process::IsAlive)
			.addMember("is_dead", &NGE::Core::Process::IsDead)
			.addMember("is_paused", &NGE::Core::Process::IsPaused)
			.addConstructor()
			);

	(*manager.GetLuaState().lock())["ScriptProcess"].setClass(kaguya::ClassMetatable<LuaScriptProcess, NGE::Core::Process>()
			.addMember("attach_child", &LuaScriptProcess::ScriptAttachChild)
			.addMember("build_data", &LuaScriptProcess::BuildDataFromScript)
			.addConstructor()
			);
}

void LuaScriptProcess::OnInit() {
	NGE::Core::Process::OnInit();
	if (!initFunction.isNilref()) {
		initFunction();
	}

	if (!updateFunction.isNilref()) {
		Fail();
	}
}

void LuaScriptProcess::OnUpdate(unsigned int deltaMs) {
	time += deltaMs;
	if (time >= frequency) {
		updateFunction(time);
		time = 0;
	}
}

void LuaScriptProcess::OnSuccess() {
	if (!successFunction.isNilref()) {
		successFunction();
	}
}

void LuaScriptProcess::OnFail() {
	if (!failFunction.isNilref()) {
		failFunction();
	}
}

void LuaScriptProcess::OnAbort() {
	if (!abortFunction.isNilref()) {
		abortFunction();
	}
}

bool LuaScriptProcess::BuildDataFromScript(kaguya::LuaRef self) {
	if (!self.isNilref()) {
		
		kaguya::LuaRef refFunction;
		refFunction = self["on_init"];
		if (!refFunction.isNilref()) {
			std::cout << "ref function not null" << std::endl;
			initFunction = refFunction;
		}

		refFunction = self["on_update"];
		if (!refFunction.isNilref()) {
			updateFunction = refFunction;
		} else {
			nge_log_error("LuaScriptProcess --> No 'on_update()' method found in script process");
			return false;
		}

		refFunction = self["on_success"];
		if (!refFunction.isNilref()) {
			successFunction = refFunction;
		}

		refFunction = self["on_fail"];
		if (!refFunction.isNilref()) {
			failFunction = refFunction;
		}
		refFunction = self["on_abort"];
		if (!refFunction.isNilref()) {
			abortFunction = refFunction;
		}

		kaguya::LuaRef refFrequency = self["frequency"];
		if (!refFrequency.isNilref()) {
			frequency = refFrequency;
		}
	} else {
		nge_log_error("LuaScriptProcess --> self is null");
		return false;
	}

	return true;
}

void LuaScriptProcess::ScriptAttachChild(kaguya::LuaRef child) {

}
