#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "NGE/Scripting/LuaScriptManager.hpp"
#include "NGE/Scripting/LuaScriptEvent.hpp"

using namespace NGE::Scripting;
namespace k = kaguya;

BOOST_AUTO_TEST_CASE(RegisterSingleEventTypeWithScript) {
	LuaScriptManager& manager = LuaScriptManager::GetInstance();
	manager.Initialise();

	LuaScriptEvent::RegisterEventTypeWithScript("TestKey", 0x12345);
	k::LuaTable eventTypeTable = (*manager.GetLuaState().lock())["EventType"];
	
	BOOST_CHECK(!eventTypeTable.isNilref());
	BOOST_CHECK_EQUAL(1, eventTypeTable.keys().size());
}

BOOST_AUTO_TEST_CASE(RegisterMultipleEventTypesWithScript) {
	LuaScriptManager& manager = LuaScriptManager::GetInstance();
	manager.Initialise();

	LuaScriptEvent::RegisterEventTypeWithScript("TestKey1", 0x12345);
	LuaScriptEvent::RegisterEventTypeWithScript("TestKey2", 0x12346);
	LuaScriptEvent::RegisterEventTypeWithScript("TestKey3", 0x12347);
	
	k::LuaTable eventTypeTable = (*manager.GetLuaState().lock())["EventType"];
	
	BOOST_CHECK(!eventTypeTable.isNilref());
	BOOST_CHECK_EQUAL(3, eventTypeTable.keys().size());
}