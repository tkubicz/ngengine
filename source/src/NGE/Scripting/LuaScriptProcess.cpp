#include "NGE/Scripting/LuaScriptProcess.hpp"
#include "NGE/Scripting/LuaScriptManager.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"
using namespace NGE::Scripting;

void LuaScriptProcess::RegisterScriptClass() {
	LuaScriptManager& manager = LuaScriptManager::GetInstance();
	(*manager.GetLuaState().lock())["Process"].setClass(kaguya::ClassMetatable<NGE::Core::Process>()
			.addMember("succeed", &NGE::Core::Process::Succeed)
			.addMember("fail", &NGE::Core::Process::Fail)
			.addMember("pause", &NGE::Core::Process::Pause)
			.addMember("unpause", &NGE::Core::Process::Run)
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

	// Make ability to create objects owned by C++ using "class" lua function.
	(*manager.GetLuaState().lock())["ScriptProcess"]["shared"] = &std::make_shared<LuaScriptProcess>;
}

void LuaScriptProcess::OnInit() {
	NGE::Core::Process::OnInit();
	if (!initFunction.isNilref()) {
		initFunction(self);
	}

	if (updateFunction.isNilref()) {
		Fail();
	}
}

void LuaScriptProcess::OnUpdate(unsigned int deltaMs) {
	time += deltaMs;
	if (time >= frequency) {
		updateFunction(self, time);
		time = 0;
	}
}

void LuaScriptProcess::OnSuccess() {
	if (!successFunction.isNilref()) {
		successFunction(self);
	}
}

void LuaScriptProcess::OnFail() {
	if (!failFunction.isNilref()) {
		failFunction(self);
	}
}

void LuaScriptProcess::OnAbort() {
	if (!abortFunction.isNilref()) {
		abortFunction(self);
	}
}

bool LuaScriptProcess::BuildDataFromScript(kaguya::LuaRef self) {
	if (!self.isNilref()) {
		kaguya::LuaRef refFunction;

		kaguya::LuaRef refInitFunction = self["on_init"];
		if (!refInitFunction.isNilref()) {
			log_trace("on_init function found");
			initFunction = refInitFunction;
		}

		kaguya::LuaRef refUpdateFunction = self["on_update"];
		if (!refUpdateFunction.isNilref()) {
			log_trace("on_update function found");
			updateFunction = refUpdateFunction;
		} else {
			log_error("No 'on_update()' method found in script process");
			return false;
		}

		refFunction = self["on_success"];
		if (!refFunction.isNilref()) {
			log_trace("on_success function found");
			successFunction = refFunction;
		}

		refFunction = self["on_fail"];
		if (!refFunction.isNilref()) {
			log_trace("on_fail function found");
			failFunction = refFunction;
		}
		refFunction = self["on_abort"];
		if (!refFunction.isNilref()) {
			log_trace("on_abort function found");
			abortFunction = refFunction;
		}

		kaguya::LuaRef refFrequency = self["frequency"];
		if (!refFrequency.isNilref()) {
			log_trace("frequency variable found");
			frequency = refFrequency;
		}

		this->self = self;
	} else {
		log_error("self is null");
		return false;
	}

	return true;
}

void LuaScriptProcess::ScriptAttachChild(kaguya::LuaRef child) {
}
