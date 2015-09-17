#include "NGE/Scripting/LuaScriptManager.hpp"
#include "NGE/Tools/Logger.hpp"
using namespace NGE::Scripting;

LuaScriptManager::LuaScriptManager() {
	luaState = nullptr;
}

LuaScriptManager::~LuaScriptManager() {
}

bool LuaScriptManager::Init() {
	luaState = std::make_shared<sel::State>(true);
	if (luaState == nullptr) {
		nge_log_error("LuaScriptManager --> Could not initialize lua state");
		return false;
	}
	return true;
}

bool LuaScriptManager::ExecuteFile(const std::string& path) {
	bool result = luaState->Load(path);
	if (!result) {
		SetError();
	}
	return result;
}

bool LuaScriptManager::ExecuteString(const std::string& str) {
	bool result;
	if (str[0] != '=') {
		result = (*luaState)(str.c_str());
		if (!result) {
			SetError();
		}
	} else {
		std::string buffer = "print(" + str.substr(1) + ")";
		result = (*luaState)(buffer.c_str());
		if (!result) {
			SetError();
		}
	}
	return result;
}

void LuaScriptManager::SetError() {
	std::string message = luaState->Read<std::string>(-1);
	if (!message.empty()) {
		lastError = message;
		luaState->Push(0);
	} else {
		lastError = "Unknown Lua error";
	}

	nge_log_error("LuaScriptManager --> " + lastError);
}

std::string LuaScriptManager::GetLastError() {
	return lastError;
}

std::weak_ptr<sel::State> LuaScriptManager::GetLuaState() {
	return std::weak_ptr<sel::State>(luaState);
}
