#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "NGE/Scripting/LuaScriptManager.hpp"
#include "NGE/Scripting/LuaScriptProcess.hpp"
#include "NGE/Core/ProcessManager.hpp"
#include "NGE/Tools/Logger.hpp"

using namespace NGE::Scripting;

BOOST_AUTO_TEST_CASE(testRegisterScriptClass) {
	NGE::Tools::Logger::Initialize("log.txt", true);

	LuaScriptManager& manager = LuaScriptManager::GetInstance();
	BOOST_CHECK(manager.Initialise());
	BOOST_CHECK(manager.ExecuteFile("../test/Data/Scripting/ngengine-lib.lua"));
//	BOOST_CHECK(manager.ExecuteFile("test/Data/Scripting/ngengine-lib.lua"));

	LuaScriptProcess::RegisterScriptClass();

	BOOST_CHECK(manager.ExecuteFile("../test/Data/Scripting/register-class.lua"));
//	BOOST_CHECK(manager.ExecuteFile("test/Data/Scripting/register-class.lua"));

	LuaScriptProcess* process = (*manager.GetLuaState().lock())["tp"]["cpp_object"];
	BOOST_CHECK(process != nullptr);
	
	std::shared_ptr<LuaScriptProcess> sharedProcess = (*manager.GetLuaState().lock())["tp"]["cpp_object"];
	
	BOOST_CHECK_EQUAL(process, sharedProcess.get());
	
	NGE::Core::ProcessManager pm;
	pm.AttachProcess(sharedProcess);

	BOOST_CHECK_EQUAL(1, pm.GetProcessCount());
	int result = pm.UpdateProcesses(10);
	std::cout << "success count: " << (result >> 16) << ", fail count: " << (result >> 16) << std::endl;
	
	result = pm.UpdateProcesses(20); // 30
	std::cout << "success count: " << (result >> 16) << ", fail count: " << (result >> 16) << std::endl;
	
	result = pm.UpdateProcesses(50); // 80
	std::cout << "success count: " << (result >> 16) << ", fail count: " << (result >> 16) << std::endl;
	
	result = pm.UpdateProcesses(80); // 160
	std::cout << "success count: " << (result >> 16) << ", fail count: " << (result >> 16) << std::endl;
	
	result = pm.UpdateProcesses(50); // 210
	std::cout << "success count: " << (result >> 16) << ", fail count: " << (result >> 16) << std::endl;
	
	pm.UpdateProcesses(50); // 260
	
	std::cout << "use count: " << sharedProcess.use_count() << std::endl;

	std::cout << manager.GetLastError() << std::endl;
}