#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>
#include <cppformat/format.h>
#include <boost/test/unit_test.hpp>
#include "TestSettings.hpp"
#include "NGE/Scripting/LuaScriptManager.hpp"

using namespace NGE::Scripting;

BOOST_AUTO_TEST_CASE(ExecuteStringTest) {
	LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
	scriptManager.Initialise();
	bool result = scriptManager.ExecuteString("print('I am executing this string and printing it!')");
	BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(ExecuteErrorString) {
	LuaScriptManager::GetInstance().Initialise();
	bool result = LuaScriptManager::GetInstance().ExecuteString("undefined()");
	BOOST_CHECK(!result);
	std::string errorMessage = LuaScriptManager::GetInstance().GetLastError();
	BOOST_CHECK_EQUAL(errorMessage, "[string \"undefined()\"]:1: attempt to call global 'undefined' (a nil value)");
}

BOOST_AUTO_TEST_CASE(ExecuteStringPrintShortcut) {
	LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
	scriptManager.Initialise();
	bool result = scriptManager.ExecuteString("='Print this message from shortcut!'");
	BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(ExecuteFile) {
	LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
	scriptManager.Initialise();

	bool result = scriptManager.ExecuteFile(fmt::format("{}/{}", TEST_ASSET_DIR, "Data/test.lua"));
	BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(ExecuteFileThatDoesntExist) {

	LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
	scriptManager.Initialise();
	bool result = scriptManager.ExecuteFile(fmt::format("{}/{}", TEST_ASSET_DIR, "Data/doesntexist.lua"));
	BOOST_CHECK(!result);
}

BOOST_AUTO_TEST_CASE(ExecuteMultipleLines) {

	LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
	scriptManager.Initialise();

	bool result = false;

	result = scriptManager.ExecuteString("a = 10");
	BOOST_CHECK(result);

	result = scriptManager.ExecuteString("print(a)");
	BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(GetLuaState) {
	LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
	scriptManager.Initialise();
	bool result = scriptManager.ExecuteFile(fmt::format("{}/{}", TEST_ASSET_DIR, "Data/test.lua"));
	BOOST_CHECK(result);

	std::weak_ptr<kaguya::State> state = scriptManager.GetLuaState();

	(*(state.lock()))("a = 15");
	(*(state.lock()))("print(a)");
	int foo = (*(state.lock()))["foo"];
	BOOST_CHECK_EQUAL(foo, 4);

	std::string msg = (*scriptManager.GetLuaState().lock())["bar"][3];
	BOOST_CHECK_EQUAL(msg, "hi");

	// When using state implicitly we need to surround the statement 
	// with try-catch block to ensure that exception will be caught.
	try {
		int value = (*state.lock())["test"];
		std::cout << value << std::endl;
	} catch (std::exception& e) {
		std::cout << "Exception!" << std::endl;
	}

	kaguya::LuaRef ref = (*state.lock())["test"];
	BOOST_CHECK(ref.isNilref());

	BOOST_CHECK_EQUAL(scriptManager.GetLuaState().use_count(), 1);
}