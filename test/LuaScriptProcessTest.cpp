#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "NGE/ThirdPart/selene.h"
#include "NGE/Scripting/LuaScriptProcess.hpp"

using namespace sel;

class TestClass {
  public:
	int value;

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

BOOST_AUTO_TEST_CASE(Test) {
	// Create lua context and load standard library.
	State state{true};

	state["ScriptProcess"].SetClass<TestClass>("print", &TestClass::printValue);
	state["AnotherProcess"].SetClass<Inherited>("set", &Inherited::setValue);
}
