#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <memory>
#include <iostream>
#include <functional>
#include <boost/test/unit_test.hpp>

using namespace std::placeholders;

typedef std::function<void(int, float) > some_func;
typedef std::pair<intptr_t, intptr_t> method_hash;

void func1(int val1, float val2) {
	std::cout << "func1" << std::endl;
}

void func2(int val1, float val2) {
	std::cout << "func2" << std::endl;
}

BOOST_AUTO_TEST_CASE(TestFunctionCompare) {
	some_func test3 = std::bind(func1, _1, _2);
	some_func test4 = std::bind(func1, _1, _2);
	some_func test5 = std::bind(func2, _1, _2);

	BOOST_CHECK(test3.target_type() == test5.target_type());

	void (*const* some_ptr)(int, float) = test3.target<void(*)(int, float) >();
	void (*const* second_ptr)(int, float) = test5.target<void(*)(int, float) >();

	BOOST_CHECK(some_ptr == second_ptr);
}