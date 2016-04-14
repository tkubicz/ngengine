#include "catch.hpp"
#include "cppformat/format.h"
#include "TestSettings.hpp"
#include "NGE/Scripting/LuaScriptManager.hpp"
#include "NGE/Scripting/LuaScriptEventListener.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"

namespace s = NGE::Scripting;
namespace l = NGE::Tools::Logger;

TEST_CASE("Development", "[lua][lua-script-event-listener]") {
	l::NewLogger& log = l::NewLogger::GetInstance();
	log["console"]->SetEnabled(true);
	log["console"]->SetAutoFlushEnabled(true);
	log["console"]->SetFlushAfter(1);
	
	s::LuaScriptManager& manager = s::LuaScriptManager::GetInstance();
	REQUIRE(manager.Initialise());
	
	s::LuaScriptEventListener::RegisterScriptEventListerClass();
	s::LuaScriptEvent::RegisterScriptClass();
	
	REQUIRE(manager.ExecuteFile(fmt::format("{}/{}", TEST_ASSET_DIR, "Data/Scripting/ngengine-lib.lua")));
	REQUIRE(manager.ExecuteFile(fmt::format("{}/{}", TEST_ASSET_DIR, "Data/Scripting/events.lua")));
}