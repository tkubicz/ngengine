#include "catch.hpp"
#include "TestSettings.hpp"
#include "NGE/Scripting/LuaScriptManager.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"
#include "NGE/Tools/Logger/Output/StreamLoggerOutput.hpp"

namespace s = NGE::Scripting;
namespace l = NGE::Tools::Logger;

SCENARIO("Initialise LuaScriptManager", "[lua][lua-script-manager]") {

	GIVEN("LuaScriptManager instance") {
		s::LuaScriptManager& lsm = s::LuaScriptManager::GetInstance();

		WHEN("LuaScriptManager is initialised") {
			bool result = lsm.Initialise();

			THEN("Initialisation was successful") {
				REQUIRE(result);
			}

			THEN("Error handling works") {

				WHEN("Error script is executed") {
					lsm.ExecuteString("print test");

					THEN("Last error is written to variable") {
						REQUIRE(lsm.GetLastError().find("syntax error near 'test'") != std::string::npos);
					}
				}
			}
		}
	}
}

SCENARIO("Execute strings", "[lua][lua-script-manager]") {

	GIVEN("LuaScriptManager instance") {
		s::LuaScriptManager& lsm = s::LuaScriptManager::GetInstance();
		REQUIRE(lsm.Initialise());

		WHEN("Executing string") {
			bool result = lsm.ExecuteString("print('Executing string and printing it')");

			THEN("String was executed properly") {
				REQUIRE(result);
			}
		}

		WHEN("Executing error string") {
			bool result = lsm.ExecuteString("undefined()");

			THEN("String was executed with error") {
				REQUIRE_FALSE(result);
				REQUIRE(lsm.GetLastError().find("attempt to call global 'undefined'") != std::string::npos);
			}
		}

		WHEN("Executing string using print shortcut") {
			bool result = lsm.ExecuteString("='Message from shortcut'");

			THEN("String was executed properly") {
				REQUIRE(result);
			}
		}
	}
}

//
//BOOST_AUTO_TEST_CASE(ExecuteFile) {
//	LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
//	scriptManager.Initialise();
//
//	bool result = scriptManager.ExecuteFile(fmt::format("{}/{}", TEST_ASSET_DIR, "Data/test.lua"));
//	BOOST_CHECK(result);
//}
//
//BOOST_AUTO_TEST_CASE(ExecuteFileThatDoesntExist) {
//
//	LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
//	scriptManager.Initialise();
//	bool result = scriptManager.ExecuteFile(fmt::format("{}/{}", TEST_ASSET_DIR, "Data/doesntexist.lua"));
//	BOOST_CHECK(!result);
//}
//
//BOOST_AUTO_TEST_CASE(ExecuteMultipleLines) {
//
//	LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
//	scriptManager.Initialise();
//
//	bool result = false;
//
//	result = scriptManager.ExecuteString("a = 10");
//	BOOST_CHECK(result);
//
//	result = scriptManager.ExecuteString("print(a)");
//	BOOST_CHECK(result);
//}
//
//BOOST_AUTO_TEST_CASE(GetLuaState) {
//	LuaScriptManager& scriptManager = LuaScriptManager::GetInstance();
//	scriptManager.Initialise();
//	bool result = scriptManager.ExecuteFile(fmt::format("{}/{}", TEST_ASSET_DIR, "Data/test.lua"));
//	BOOST_CHECK(result);
//
//	std::weak_ptr<kaguya::State> state = scriptManager.GetLuaState();
//
//	(*(state.lock()))("a = 15");
//	(*(state.lock()))("print(a)");
//	int foo = (*(state.lock()))["foo"];
//	BOOST_CHECK_EQUAL(foo, 4);
//
//	std::string msg = (*scriptManager.GetLuaState().lock())["bar"][3];
//	BOOST_CHECK_EQUAL(msg, "hi");
//
//	// When using state implicitly we need to surround the statement 
//	// with try-catch block to ensure that exception will be caught.
//	try {
//		int value = (*state.lock())["test"];
//		std::cout << value << std::endl;
//	} catch (std::exception& e) {
//		std::cout << "Exception!" << std::endl;
//	}
//
//	kaguya::LuaRef ref = (*state.lock())["test"];
//	BOOST_CHECK(ref.isNilref());
//
//	BOOST_CHECK_EQUAL(scriptManager.GetLuaState().use_count(), 1);
//}