#include "catch.hpp"
#include "cppformat/format.h"
#include "TestSettings.hpp"
#include "NGE/Scripting/LuaScriptManager.hpp"
#include "NGE/Scripting/LuaScriptEventListener.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"

namespace e = NGE::Events;
namespace s = NGE::Scripting;
namespace l = NGE::Tools::Logger;
namespace k = kaguya;

l::LogConfig logConfig;

void enableConsoleLogging() {
	l::NewLogger& log = l::NewLogger::GetInstance();
	logConfig = log["console"]->GetLogConfig();
	log["console"]->SetEnabled(true);
	log["console"]->SetAutoFlushEnabled(true);
	log["console"]->SetFlushAfter(1);
}

void disableConsoleLogging() {
	l::NewLogger& log = l::NewLogger::GetInstance();
	log["console"]->SetLogConfig(logConfig);
}

SCENARIO("Register event listener from script", "[lua][lua-script-event-listener]") {

	GIVEN("LuaScriptManager with NGE library") {
		enableConsoleLogging();
		s::LuaScriptManager& manager = s::LuaScriptManager::GetInstance();
		REQUIRE(manager.Initialise());
		REQUIRE(manager.ExecuteFile(fmt::format("{}/{}", TEST_ASSET_DIR, "Data/Scripting/ngengine-lib.lua")));

		WHEN("LuaScriptEventListener class is registered") {
			s::LuaScriptEvent::RegisterScriptClass();
			s::LuaScriptEvent::RegisterEventTypeWithScript("MouseEvent", e::MouseEvent::eventType);
			s::LuaScriptEventListener::RegisterScriptClass();

			WHEN("Events script is loaded") {
				REQUIRE(manager.ExecuteFile(fmt::format("{}/{}", TEST_ASSET_DIR, "Data/Scripting/events.lua")));

				WHEN("Event manager is updated") {
					e::EventManager& eventManager = e::EventManager::GetInstance();
					
					std::shared_ptr<e::MouseEvent> mouseEventPtr = std::make_shared<e::MouseEvent>();
					eventManager.QueueEvent(mouseEventPtr);
					eventManager.Update();
				}
			}
		}

		disableConsoleLogging();
	}
}