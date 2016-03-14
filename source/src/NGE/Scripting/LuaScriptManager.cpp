#include "NGE/Scripting/LuaScriptManager.hpp"
#include "NGE/Core/Delegate.hpp"
#include "NGE/Tools/Logger.hpp"
using namespace NGE::Scripting;

LuaScriptManager::LuaScriptManager() {
    luaState = nullptr;
}

bool LuaScriptManager::Initialise() {
    luaState = std::make_shared<kaguya::State>();
    if (luaState == nullptr) {
        nge_log_error("LuaScriptManager --> Could not initialise Lua state");
        return false;
    }

    //	std::function<void(int, const char*) > delegate = NGE::Core::make_delegate(GetInstance(), &LuaScriptManager::HandleError);
    //	luaState->setErrorHandler(delegate);

    luaState->setErrorHandler(&LuaScriptManager::HandleError);

    return true;
}

bool LuaScriptManager::ExecuteFile(const std::string& path) {
    bool result = luaState->dofile(path);
    if (!result) {
        nge_log_error("LuaScriptManager --> Could not execute file: " + nge_to_string(path));
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
    LuaScriptManager::GetInstance().SetLastError(msg);
    nge_log_error("LuaScriptManager --> " + nge_to_string(msg));
}

void LuaScriptManager::SetLastError(const char* error) {
    lastError = error;
}

std::string LuaScriptManager::GetLastError() const {
    return lastError;
}

std::weak_ptr<kaguya::State> LuaScriptManager::GetLuaState() {
    return luaState;
}
