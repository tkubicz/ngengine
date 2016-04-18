#include "catch.hpp"
#include "TestSettings.hpp"
#include "NGE/Scripting/LuaScriptManager.hpp"
#include "NGE/Scripting/LuaScriptProcess.hpp"
#include "NGE/Core/ProcessManager.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"

namespace s = NGE::Scripting;
namespace l = NGE::Tools::Logger;

SCENARIO("Create process using script", "[lua][lua-script-process]") {

	GIVEN("LuaScriptManager") {
		s::LuaScriptManager& manager = s::LuaScriptManager::GetInstance();
		REQUIRE(manager.Initialise());

		//		l::NewLogger& log = l::NewLogger::GetInstance();
		//		log["console"]->SetEnabled(true);
		//		log["console"]->SetAutoFlushEnabled(true);
		//		log["console"]->SetFlushAfter(1);

		WHEN("NGE library is loaded") {
			REQUIRE(manager.ExecuteFile(fmt::format("{}/{}", TEST_ASSET_DIR, "Data/Scripting/ngengine-lib.lua")));

			WHEN("LuaScriptProcess is registered") {
				s::LuaScriptProcess::RegisterScriptClass();

				WHEN("Create process script is loaded") {
					REQUIRE(manager.ExecuteFile(fmt::format("{}/{}", TEST_ASSET_DIR, "Data/Scripting/create-script-process.lua")));

					THEN("It is possible to get raw and shared pointers") {
						s::LuaScriptProcess* rawPointer = (*manager.GetLuaState().lock())["tp"]["cpp_object"];
						REQUIRE(rawPointer != nullptr);
						std::shared_ptr<s::LuaScriptProcess> sharedPointer = (*manager.GetLuaState().lock())["tp"]["cpp_object"];
						REQUIRE(sharedPointer.get() != nullptr);

						THEN("Pointers are the same") {
							REQUIRE(rawPointer == sharedPointer.get());
						}

						WHEN("Process is attached to ProcessManager") {
							NGE::Core::ProcessManager pm;
							pm.AttachProcess(sharedPointer);

							int result = 0;
							for (int i = 0; i < 5; ++i) {
								result = pm.UpdateProcesses(50);
							}

							int successCount = result >> 16;
							int failureCount = result & 0xFFFF;

							THEN("Process finished successfully") {
								REQUIRE(successCount == 1);
								REQUIRE(failureCount == 0);
								bool success = (*manager.GetLuaState().lock())["success"];
								REQUIRE(success);
							}
						}
					}
				}
			}
		}
	}
}
