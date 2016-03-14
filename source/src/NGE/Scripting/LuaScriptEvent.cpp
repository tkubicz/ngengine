#include "NGE/Scripting/LuaScriptEvent.hpp"
#include "NGE/Scripting/LuaScriptManager.hpp"

using namespace NGE::Scripting;
namespace k = kaguya;

const NGE::Events::EventType LuaScriptEvent::eventType(0x0f4bf9b3);

void LuaScriptEvent::RegisterScriptClass() {
    LuaScriptManager& manager = LuaScriptManager::GetInstance();
    (*manager.GetLuaState().lock())["Event"].setClass(k::ClassMetatable<NGE::Scripting::LuaScriptEvent>()
            .addConstructor()
            );
}
