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

	LuaScriptProcess::RegisterScriptClass();

	BOOST_CHECK(manager.ExecuteFile("../test/Data/Scripting/register-class.lua"));

	LuaScriptProcess* process = (*manager.GetLuaState().lock())["tp"]["cpp_object"];
	BOOST_CHECK(process != nullptr);
	
	//process->OnInitWrapper();
	//process->OnUpdateWrapper(130);
	//process->OnSuccessWrapper();

	NGE::Core::ProcessManager pm;
	//	std::shared_ptr<NGE::Core::Process> ptr = process->GetSharedPtr();
	//	pm.AttachProcess(ptr);
	//	
	//	BOOST_CHECK_EQUAL(1, pm.GetProcessCount());
	//	pm.UpdateProcesses(200);

	//BOOST_CHECK(ptr.get() == process);

	std::cout << manager.GetLastError() << std::endl;
}