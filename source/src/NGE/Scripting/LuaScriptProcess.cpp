#include "NGE/Scripting/LuaScriptProcess.hpp"
#include "NGE/Scripting/LuaScriptManager.hpp"
#include "NGE/Scripting/LuaScriptEventListener.hpp"
#include "NGE/Core/Delegate.hpp"
using namespace NGE::Scripting;

LuaScriptProcess::LuaScriptProcess() {
    frequency = 10;
    time = 0;
}

void LuaScriptProcess::OnInit() { }

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

void LuaScriptProcess::OnAbort() { }

bool LuaScriptProcess::BuildProcessFromScript(const std::string& name) {
    LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
    std::weak_ptr<sel::State> luaState = scriptManager.GetLuaState();

    scriptInitFunction = (*luaState.lock())["process"]["OnInit"];
    scriptUpdateFunction = (*luaState.lock())["process"]["OnUpdate"];
    scriptSuccessFunction = (std::function<void() >) (*luaState.lock())["process"]["OnSuccess"];

    return true;
}

void LuaScriptProcess::ScriptAttachChild(void* self, const std::string childVariableName) {
    nge_log_info("LuaScriptProcess --> Attaching child \"" + childVariableName + "\"");
 }


void LuaScriptProcess::RegisterScriptClass() {
    LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
    std::weak_ptr<sel::State> luaState = scriptManager.GetLuaState();
    (*luaState.lock())["CreateProcess"] = &NGE::Scripting::LuaScriptProcess::CreateFromScript;
}

void LuaScriptProcess::CreateFromScript(const std::string className, const std::string variableName) {
    LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
    std::weak_ptr<sel::State> luaState = scriptManager.GetLuaState();

    LuaScriptProcess* process = new LuaScriptProcess();
    std::string command = variableName + " = " + className + "()";
    nge_log_info("Creating new process variable: " + command);
    (*luaState.lock())(command.c_str());

    std::function<void() > succeedFunc = NGE::Core::make_delegate(*process, &LuaScriptProcess::Succeed);
    (*luaState.lock())[variableName.c_str()]["Succeed"] = succeedFunc;

    std::function<void() > failFunc = NGE::Core::make_delegate(*process, &LuaScriptProcess::OnFail);
    (*luaState.lock())[variableName.c_str()]["Fail"] = failFunc;

    std::function<void() > pauseFunc = NGE::Core::make_delegate(*process, &LuaScriptProcess::Pause);
    (*luaState.lock())[variableName.c_str()]["Pause"] = pauseFunc;

    std::function<void() > unPauseFunc = NGE::Core::make_delegate(*process, &LuaScriptProcess::UnPause);
    (*luaState.lock())[variableName.c_str()]["UnPause"] = unPauseFunc;
    
    std::function<void(void*, const std::string)> attachChildFunc = NGE::Core::make_delegate(*process, &LuaScriptProcess::ScriptAttachChild);
    (*luaState.lock())[variableName.c_str()]["AttachChild"] = attachChildFunc;
    
    process->scriptInitFunction = (*luaState.lock())[variableName.c_str()]["OnInit"];
    process->scriptUpdateFunction = (*luaState.lock())[variableName.c_str()]["OnUpdate"];
    process->scriptSuccessFunction = (*luaState.lock())[variableName.c_str()]["OnSuccess"];
    process->scriptFailFunction = (*luaState.lock())[variableName.c_str()]["OnFail"];
    process->scriptAbortFunction = (*luaState.lock())[variableName.c_str()]["OnAbort"];
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

