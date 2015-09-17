#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "NGE/ThirdPart/selene.h"
#include "NGE/Scripting/LuaScriptProcess.hpp"

using namespace sel;

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
}
