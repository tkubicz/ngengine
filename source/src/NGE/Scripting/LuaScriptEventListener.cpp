#include "NGE/Scripting/LuaScriptEventListener.hpp"
using namespace NGE::Scripting;

LuaScriptEventListener::LuaScriptEventListener(const std::string& eventDelelgateId, const NGE::Events::EventType& eventType, const std::function<void()() > scriptCallbackFunction) {
    this->eventDelegateId = eventDelelgateId;
    this->eventType = eventType;
    this->scriptCallbackFunction = scriptCallbackFunction;
}

LuaScriptEventListener::~LuaScriptEventListener() {
    NGE::Events::IEventManager* eventManage = NGE::Events::IEventManager::Get();
    if (eventManage) {
        eventManage->RemoveListener(eventDelegateId, eventType);
    }
}

NGE::Events::EventListenerDelegate LuaScriptEventListener::GetDelegate() {
    return NGE::Core::make_delegate(*this, &LuaScriptEventListener::ScriptEventDelegate);
}

void LuaScriptEventListener::ScriptEventDelegate(NGE::Events::IEventDataPtr eventPtr) {
    if (scriptCallbackFunction) {
        // Call the Lua function.
        scriptCallbackFunction(eventPtr);
    }
}




