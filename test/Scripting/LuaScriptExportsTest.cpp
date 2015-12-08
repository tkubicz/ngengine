#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "NGE/ThirdPart/selene.h"
#include "NGE/Scripting/LuaScriptProcess.hpp"
#include "NGE/Scripting/LuaScriptManager.hpp"
#include "NGE/Scripting/LuaScriptExports.hpp"
#include "NGE/Events/IEventData.hpp"

using namespace sel;
using namespace NGE::Scripting;

BOOST_AUTO_TEST_CASE(TestRegister) {
	LuaScriptManager& manager = LuaScriptManager::GetInstance();
	BOOST_CHECK(manager.Init());

	std::weak_ptr<sel::State> luaState = manager.GetLuaState();
	
	LuaScriptExports::Register();

	std::string key = "TestKey";
	NGE::Events::EventType eventType = 12345;

	(*luaState.lock())["RegisterEventType"](key, (unsigned int) eventType);
	
	unsigned int eventTypeFromScript = (*luaState.lock())["EventType"][key.c_str()];
	BOOST_CHECK_EQUAL(eventType, eventTypeFromScript);
}
