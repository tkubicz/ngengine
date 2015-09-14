#include "NGE/Scripting/LuaScriptManager.hpp"
#include "NGE/Tools/Logger.hpp"
using namespace NGE::Scripting;

bool LuaScriptManager::Init() {
	luaState = new sel::State{true};
	if (luaState == nullptr) {
		log_error("LuaScriptManager --> Could not initialize lua state");
		return false;
	}
}

void LuaScriptManager::ExecuteFile(const std::string& resource) {
}

void LuaScriptManager::ExecuteString(const std::string& str) {
}




