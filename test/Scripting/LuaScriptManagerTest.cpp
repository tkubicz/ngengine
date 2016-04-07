#include "catch.hpp"
#include "cppformat/format.h"
#include "TestSettings.hpp"
#include "NGE/Scripting/LuaScriptManager.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"
#include "NGE/Tools/Logger/Output/StreamLoggerOutput.hpp"

namespace s = NGE::Scripting;
namespace l = NGE::Tools::Logger;
namespace k = kaguya;

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
			bool result = lsm.ExecuteString("a = 20");

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
			bool result = lsm.ExecuteString("='msg'");

			THEN("String was executed properly") {
				REQUIRE(result);
			}
		}

		WHEN("Multiple strings are executed") {
			bool first = lsm.ExecuteString("a = 10");
			bool second = lsm.ExecuteString("b = a + 4");

			THEN("State is the same") {
				REQUIRE(first);
				REQUIRE(second);
			}
		}
	}
}

SCENARIO("Execute files", "[lua][lua-script-manager]") {

	GIVEN("LuaScriptManager instance") {
		s::LuaScriptManager& lsm = s::LuaScriptManager::GetInstance();
		REQUIRE(lsm.Initialise());

		WHEN("Executing file") {
			bool result = lsm.ExecuteFile(fmt::format("{}/{}", TEST_ASSET_DIR, "Data/test.lua"));

			THEN("File was executed successfully") {
				REQUIRE(result);
			}
		}

		WHEN("Executing file that doesn't exist") {
			bool result = lsm.ExecuteFile(fmt::format("{}/{}", TEST_ASSET_DIR, "Data/doesntexist.lua"));

			THEN("File was not executed") {
				REQUIRE_FALSE(result);
			}
		}
	}
}

SCENARIO("Get Lua state", "[lua][lua-script-manager]") {

	GIVEN("LuaScriptManager instance") {
		s::LuaScriptManager& lsm = s::LuaScriptManager::GetInstance();
		REQUIRE(lsm.Initialise());

		WHEN("Some script is executed") {
			bool result = lsm.ExecuteFile(fmt::format("{}/{}", TEST_ASSET_DIR, "Data/test.lua"));
			REQUIRE(result);

			WHEN("Getting pointer to Lua state") {
				std::weak_ptr<k::State> state = lsm.GetLuaState();

				THEN("We have a pointer to Lua state") {
					REQUIRE_FALSE(state.expired());
					std::shared_ptr<k::State> s = state.lock();
					
					WHEN("We execute something on state") {
						(*s)("a = 15");
						(*s)("b = a + 20");
						int a = (*s)["a"];
						int b = (*s)["b"];
						int foo = (*s)["foo"];
						std::string msg = (*s)["bar"][3];

						THEN("It works") {
							REQUIRE(a == 15);
							REQUIRE(b == 35);
							REQUIRE(foo == 4);
							REQUIRE(msg == "hi");
						}
					}
				}
			}
		}
	}
}
