#include "NGE/Scripting/LuaScriptEventListener.hpp"
using namespace NGE::Scripting;

LuaScriptEventListener::LuaScriptEventListener(const NGE::Events::EventDelegate& scriptDelegate, const NGE::Events::EventType& eventType) {
	this->scriptDelegate = scriptDelegate;
	this->eventType = eventType;
}

LuaScriptEventListener::~LuaScriptEventListener() {
	NGE::Events::IEventManager* eventManage = NGE::Events::IEventManager::Get();
	if (eventManage) {
		eventManage->RemoveListener(scriptDelegate, eventType);
	}
}

NGE::Events::EventListenerDelegate LuaScriptEventListener::GetDelegate() {
	return NGE::Core::make_delegate(*this, &LuaScriptEventListener::ScriptEventDelegate);
}

std::string LuaScriptEventListener::GetDelegateId() const {
	return scriptDelegate.GetEventDelegateId();
}

void LuaScriptEventListener::ScriptEventDelegate(NGE::Events::IEventDataPtr eventPtr) {
	if (scriptDelegate.GetEventListenerDelegate() != nullptr) {
		// Call the Lua function.
		scriptDelegate.GetEventListenerDelegate()(eventPtr);
	}
}




