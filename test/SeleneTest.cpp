#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "NGE/ThirdPart/selene.h"

using namespace sel;

BOOST_AUTO_TEST_CASE(CreateLuaContext) {
	// Create lua context.
	State state;

	// Create lua context and load standard library.
	State state2{true};
}

BOOST_AUTO_TEST_CASE(AccessingElements) {
	State state;
	bool loadResult = state.Load("../test/data/test.lua");
	BOOST_CHECK_EQUAL(loadResult, true);

	int foo = (int) state["foo"];
	BOOST_CHECK_EQUAL(foo, 4);

	std::string bar3 = state["bar"][3];
	BOOST_CHECK_EQUAL(bar3, "hi");

	std::string barKey = state["bar"]["key"];
	BOOST_CHECK_EQUAL(barKey, "three");
}

BOOST_AUTO_TEST_CASE(CallingLuaFunctionFromCpp) {
	State state;
	bool loadResult = state.Load("../test/data/function.lua");
	BOOST_CHECK_EQUAL(loadResult, true);

	// Call function with no arguments or returns.
	state["foo"]();

	// Call function with two arguments that returns an int.
	// The type parameter can be one of int, lua_Number, std::string, bool or
	// unsigned int.
	int result = state["add"](5, 2);
	BOOST_CHECK_EQUAL(result, 7);

	// Call function that returns multiple values.
	int sum, difference;
	tie(sum, difference) = state["sum_and_difference"](3, 1);
	BOOST_CHECK(sum == 4 && difference == 2);

	// Call function in table.
	result = state["mytable"]["foo"]();
	BOOST_CHECK_EQUAL(result, 4);
}

int my_multiply(int a, int b) {
	return a * b;
}

BOOST_AUTO_TEST_CASE(CallingFreeStandingCppFunctionsFromLua) {
	State state;

	// Register the function to the lua global c_multiply.
	state["c_multiply"] = &my_multiply;

	// Call the function from lua.
	int result = state["c_multiply"](4, 5);
	BOOST_CHECK_EQUAL(result, 20);
}

int take_fun_arg(sel::function<int(int, int) > fun, int a, int b) {
	return fun(a, b);
}

//BOOST_AUTO_TEST_CASE(AcceptingLuaFunctionsAsArguments) {
//	State state;
//	
//	state["take_fun_arg"] = &take_fun_arg;
//	bool loadResult = state.Load("../test/data/function_as_argument.lua");
//	BOOST_CHECK_EQUAL(loadResult, true);
//	
//	int result = state["pass_add"](3, 5);
//	BOOST_CHECK_EQUAL(result, 8);
//}

BOOST_AUTO_TEST_CASE(RunningArbitraryCode) {
	State state;
	state("x = 5");
	int val = state["x"];
	BOOST_CHECK_EQUAL(val, 5);
}

BOOST_AUTO_TEST_CASE(RunningErrorCode) {
	State state;
	bool result = state("x = this is test");
	
	std::string message = state.Read<std::string>(-1);
	std::cout << message << std::endl;
	
	BOOST_CHECK_EQUAL(result, false);
}

struct Bar {
	int x;

	Bar(int x) : x(x) {
	}

	int AddThis(int y) {
		return x + y;
	}
};

BOOST_AUTO_TEST_CASE(RegisteringClasses) {
	State state;

	state["Bar"].SetClass<Bar, int>("add_this", &Bar::AddThis);

	bool loadResult = state.Load("../test/data/registering_classes.lua");
	BOOST_CHECK_EQUAL(loadResult, true);

	int result = state["x"];
	BOOST_CHECK_EQUAL(result, 7);
}

BOOST_AUTO_TEST_CASE(RegisteringClassMemberVariables) {
	State state;
	state["Bar"].SetClass<Bar, int>("x", &Bar::x);

	state("bar = Bar.new(4)");
	
	state("x = bar:x()");
	int result = state["x"];
	BOOST_CHECK_EQUAL(result, 4);
	
	state("bar:set_x(-4)");
	state("y = bar:x()");
	result = state["y"];
	BOOST_CHECK_EQUAL(result, -4);
}

struct Foo {
	int x;

	Foo(int x) : x(x) {
	}

	int DoubleAdd(int y) {
		return 2 * (x + y);
	}

	void SetX(int x) {
		this->x = x;
	}
};

BOOST_AUTO_TEST_CASE(RegisteringObjectInstances) {
	State state;

	Foo foo(2);

	state["foo"].SetObj(foo,
			"double_add", &Foo::DoubleAdd,
			"x", &Foo::x);
	
	int result = state["foo"]["x"]();
	BOOST_CHECK_EQUAL(result, 2);
	
	state["foo"]["set_x"](4);
	BOOST_CHECK_EQUAL(foo.x, 4);
	
	result = state["foo"]["double_add"](3);
	BOOST_CHECK_EQUAL(result, 14);
}

BOOST_AUTO_TEST_CASE(GetFunctionFromLua) {
	State state;
	bool loadResult = state.Load("../test/data/function.lua");
	BOOST_CHECK(loadResult);
	
	sel::function<int(int, int)> addFunc = state["add"];
	std::function<int(int, int)> addFunc2 = state["add"];
	
	state["test"];
	
	BOOST_CHECK_EQUAL(addFunc(1, 2), 3);
	BOOST_CHECK_EQUAL(addFunc2(5, 3), 8);
}

BOOST_AUTO_TEST_CASE(SelectorTest) {
	State state;
	bool loadResult = state.Load("../test/data/function.lua");
	BOOST_CHECK(loadResult);
	
	sel::Selector selector = state["add"];
	
	typedef std::function<int(int, int)> AddFunc;
	AddFunc add = (AddFunc) selector;
	
	BOOST_CHECK_EQUAL(add(5, 5), 10);
}