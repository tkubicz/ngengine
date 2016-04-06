#include "NGE/Scripting/LuaScriptManager.hpp"
#include "NGE/Core/Delegate.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"
using namespace NGE::Scripting;

namespace c = NGE::Core;

LuaScriptManager::LuaScriptManager() {
	luaState = nullptr;
}

bool LuaScriptManager::Initialise() {
	log_debug("Initialising Lua state");
	luaState = std::make_shared<kaguya::State>();

	if (luaState == nullptr) {
		log_error("Could not initialise Lua state");
		return false;
	}

	luaState->setErrorHandler(c::make_delegate(GetInstance(), &LuaScriptManager::HandleError));
	return true;
}

bool LuaScriptManager::ExecuteFile(const std::string& path) {
	bool result = luaState->dofile(path);
	if (!result) {
		log_error("Could not execute file: '{}'", path);
	}
	return result;
}

bool LuaScriptManager::ExecuteString(const std::string& str) {
	bool result;
	if (str[0] != '=') {
		result = (*luaState.get())(str.c_str());
	} else {
		std::string buffer = "print(" + str.substr(1) + ")";
		result = (*luaState.get())(buffer.c_str());
	}
	return result;
}

void LuaScriptManager::HandleError(int val, const char* msg) {
	lastError = msg;
	log_error("Lua error: {}", msg);
}

std::string LuaScriptManager::GetLastError() const {
	return lastError;
}

std::weak_ptr<kaguya::State> LuaScriptManager::GetLuaState() {
	return luaState;
}
