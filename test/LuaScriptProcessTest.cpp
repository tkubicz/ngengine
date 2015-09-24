#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "NGE/ThirdPart/selene.h"
#include "NGE/Scripting/LuaScriptProcess.hpp"
#include "NGE/Scripting/LuaScriptManager.hpp"
#include "NGE/Core/Process.hpp"
#include "NGE/Tools/Logger.hpp"

using namespace sel;
using namespace NGE::Scripting;

namespace NGE {
	namespace Process {

		class TestClass {
		  public:
			enum TestEnum {
				TEST_VAL_1,
				TEST_VAL_2
			};
			
			int value;
			
			std::shared_ptr<int> testPointer;

			int printValue() {
				return value;
			}
		};

		class Inherited : public TestClass {
		  public:

			void setValue(int v) {
				value = v;
			}
		};
	}
}

BOOST_AUTO_TEST_CASE(Test) {
	// Create lua context and load standard library.
	State state{true};

	state["ScriptProcess"].SetClass<NGE::Process::TestClass>("print", &NGE::Process::TestClass::printValue);
	state["AnotherProcess"].SetClass<NGE::Process::Inherited>("set", &NGE::Process::Inherited::setValue);
	//state["Process"].SetClass<NGE::Scripting::LuaScriptProcess>("OnInit", &NGE::Scripting::LuaScriptProcess::OnInit);
}

BOOST_AUTO_TEST_CASE(TestRegisterClass) {
	LuaScriptProcess luaProcess;
	LuaScriptManager& luaManager = LuaScriptManager::GetInstance();
	BOOST_CHECK(luaManager.Init());
	
	NGE::Tools::Logger::Initialize("output.txt", true);
	
	luaProcess.RegisterScriptClass();
	
	(*luaManager.GetLuaState().lock()).Load("../test/data/script_process.lua");
	
	//LuaScriptProcess* fromLua = (*luaManager.GetLuaState().lock())["proc"];
	//BOOST_CHECK(fromLua != nullptr);
	
	//std::cout << "state: " << fromLua->GetState() << std::endl;
}

BOOST_AUTO_TEST_CASE(TestLuaNumber) {
    NGE::Tools::Logger::Initialize("output.txt", true);
	LuaScriptProcess luaProcess;
	LuaScriptManager& luaManager = LuaScriptManager::GetInstance();
	BOOST_CHECK(luaManager.Init());
    
    luaProcess.RegisterScriptClass();
    
    (*luaManager.GetLuaState().lock()).Load("../test/data/lib.lua");
    
    auto proc = (*luaManager.GetLuaState().lock())["proc"];
}