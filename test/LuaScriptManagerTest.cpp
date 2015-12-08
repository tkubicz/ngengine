#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "NGE/Scripting/LuaScriptManager.hpp"

using namespace sel;
using namespace NGE::Scripting;

BOOST_AUTO_TEST_CASE(ExecuteStringTest) {
	LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
	scriptManager.Init();
	bool result = scriptManager.ExecuteString("print('I am executing this string and printing it!')");
	BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(ExecuteErrorString) {
	LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
	scriptManager.Init();
	bool result = scriptManager.ExecuteString("undefined()");
	BOOST_CHECK(!result);
	std::string errorMessage = scriptManager.GetLastError();
	BOOST_CHECK_EQUAL(errorMessage, "[string \"undefined()\"]:1: attempt to call global 'undefined' (a nil value)");
}

BOOST_AUTO_TEST_CASE(ExecuteStringPrintShortcut) {
	LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
	scriptManager.Init();
	bool result = scriptManager.ExecuteString("='Print this message from shortcut!'");
	BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(ExecuteFile) {
	LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
	scriptManager.Init();
	bool result = scriptManager.ExecuteFile("../test/data/test.lua");
	BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(ExecuteFileThatDoesntExist) {
	LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
	scriptManager.Init();
	bool result = scriptManager.ExecuteFile("../test/data/doesntexist.lua");
	BOOST_CHECK(!result);
}

BOOST_AUTO_TEST_CASE(GetLuaState) {
	LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
	scriptManager.Init();
	bool result = scriptManager.ExecuteFile("../test/data/test.lua");
	BOOST_CHECK(result);

	std::weak_ptr<sel::State> state = scriptManager.GetLuaState();
	int foo = (*state.lock())["foo"];
	BOOST_CHECK_EQUAL(foo, 4);

	std::string msg = (*scriptManager.GetLuaState().lock())["bar"][3];
	BOOST_CHECK_EQUAL(msg, "hi");

	BOOST_CHECK_EQUAL(scriptManager.GetLuaState().use_count(), 1);
}