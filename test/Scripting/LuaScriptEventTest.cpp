#include "catch.hpp"
#include "NGE/Scripting/LuaScriptManager.hpp"
#include "NGE/Scripting/LuaScriptEvent.hpp"

namespace s = NGE::Scripting;
namespace k = kaguya;

SCENARIO("Register EventType within script", "[lua][lua-script-event]") {

	GIVEN("LuaScriptManager") {
		s::LuaScriptManager& manager = s::LuaScriptManager::GetInstance();
		REQUIRE(manager.Initialise());

		WHEN("Single event type is registered") {
			s::LuaScriptEvent::RegisterEventTypeWithScript("TestKey", 0x12345);

			THEN("EventType table exists within script") {
				k::LuaTable eventTypeTable = (*manager.GetLuaState().lock())["EventType"];
				REQUIRE_FALSE(eventTypeTable.isNilref());
				REQUIRE(eventTypeTable.keys().size() == 1);
			}
		}

		WHEN("Multiple event types are registered") {
			s::LuaScriptEvent::RegisterEventTypeWithScript("TestKey1", 0x12345);
			s::LuaScriptEvent::RegisterEventTypeWithScript("TestKey2", 0x12346);
			s::LuaScriptEvent::RegisterEventTypeWithScript("TestKey3", 0x12347);

			THEN("EventType table exist within script") {
				k::LuaTable eventTypeTable = (*manager.GetLuaState().lock())["EventType"];
				REQUIRE_FALSE(eventTypeTable.isNilref());
				REQUIRE(eventTypeTable.keys().size() == 3);
			}
		}
	}
}
