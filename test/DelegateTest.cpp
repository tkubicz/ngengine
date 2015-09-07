#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <memory>
#include <boost/test/unit_test.hpp>
#include "NGE/Core/Delegate.hpp"
using namespace NGE::Core;

class Foo {
  public:

	int bar(int x, float y, bool z) {
		//std::cout << "bar: " << x << " " << y << " " << z << std::endl;
		return x;
	}
};

int baz(int x, float y, bool z) {
	//std::cout << "bar: " << x << " " << y << " " << z << std::endl;
	return x;
}

BOOST_AUTO_TEST_CASE(CreateDelegate) {
	Foo foo;
	std::function<int(int, float, bool) > func[] = {
		baz,
		make_delegate(foo, &Foo::bar)
	};

	for (auto& x : func) {
		x(1, 1.0, true);
	}

	BOOST_CHECK_EQUAL(func[0](1, 2, false), 1);
}

BOOST_AUTO_TEST_CASE(AssigneFunction) {
	Foo foo;
	auto func = make_delegate(foo, &Foo::bar);
	BOOST_CHECK_EQUAL(func(2, 5, true), 2);

	std::function<int(int, float, bool) > func2 = baz;
	BOOST_CHECK_EQUAL(func2(5, 8, false), 5);
}
