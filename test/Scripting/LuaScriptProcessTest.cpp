#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "NGE/Scripting/LuaScriptManager.hpp"
#include "NGE/Scripting/LuaScriptProcess.hpp"
#include "NGE/Core/ProcessManager.hpp"

using namespace NGE::Scripting;

BOOST_AUTO_TEST_CASE(testRegisterScriptClass) {
	LuaScriptManager& manager = LuaScriptManager::GetInstance();
	BOOST_CHECK(manager.Init());
	BOOST_CHECK(manager.ExecuteFile("../test/Data/Scripting/ngengine-lib.lua"));

	LuaScriptProcess::RegisterScriptClass();

	BOOST_CHECK(manager.ExecuteFile("../test/Data/Scripting/register-class.lua"));

	LuaScriptProcess* process = (*manager.GetLuaState().lock())["tp"]["cppObject"];
	BOOST_CHECK(process != nullptr);
	
	//(*manager.GetLuaState().lock())["tp"]["on_init"]();
	
	process->OnInitWrapper();
	process->OnUpdateWrapper(10);
	
	//NGE::Core::ProcessManager pm;
	//pm.AttachProcess(sharedProcess);

	std::cout << manager.GetLastError() << std::endl;
}