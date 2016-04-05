#include "catch.hpp"
#include "NGE/Core/Singleton.hpp"
#include "NGE/Core/Delegate.hpp"
using namespace NGE::Core;

class Foo {
  public:

	Foo() {
		value = 10;
	}

	int value;

	int bar(int x) {
		return x;
	}

	int getValue() const {
		return value;
	}
};

SCENARIO("Make delegate", "[delegate]") {

	GIVEN("Instance of the test class Foo") {
		Foo foo;

		WHEN("Delegate to class method is created") {
			auto func = make_delegate(foo, &Foo::bar);

			THEN("Delegate can be used to invoke function") {
				REQUIRE(func(10) == 10);
			}
		}

		WHEN("Delegate to function is created") {
			std::function<int(int) > func = make_delegate(foo, &Foo::bar);

			THEN("Delegate can be used as std::function") {
				REQUIRE(func(10) == 10);

				AND_THEN("Delegate can be assigned to another std::function") {
					std::function<int(int) > anotherFunc = func;
					REQUIRE(anotherFunc(15) == 15);
				}
			}
		}

		WHEN("Delegate to function is created using pointer") {
			Foo* ptrFoo = new Foo();
			auto func = make_delegate(*ptrFoo, &Foo::bar);

			THEN("Delegate can be used as std::function") {
				REQUIRE(func(20) == 20);
			}
		}
	}
}

class TestSingleton : public NGE::Core::Singleton<TestSingleton> {
	friend class NGE::Core::Singleton<TestSingleton>;

  private:
	unsigned int value;

	TestSingleton() {
	}

	~TestSingleton() {
	}

  public:

	unsigned int GetValue() {
		return value;
	}

	void SetValue(unsigned int value) {
		this->value = value;
	}

};

SCENARIO("Make delegate to singleton method", "[delegate]") {

	GIVEN("Instance of TestSingleton") {
		TestSingleton& ts = TestSingleton::GetInstance();

		WHEN("Delegate to GetValue and SetValue are created") {
			auto get = make_delegate(ts, &TestSingleton::GetValue);
			auto set = make_delegate(ts, &TestSingleton::SetValue);

			WHEN("Set and get are used") {
				set(10);

				THEN("They operate on the same object") {
					REQUIRE(get() == 10);
					REQUIRE(ts.GetValue() == 10);
				}
			}
		}
	}
}