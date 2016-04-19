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

	int baz(int x) {
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

SCENARIO("Compare delegates", "[delegate]") {
	Foo foo;

	GIVEN("Two the same deleagetes") {
		auto foo1 = make_delegate(foo, &Foo::bar);
		auto foo2 = make_delegate(foo, &Foo::bar);

		WHEN("Delelgates are compared") {
			bool equals = (foo1 == foo2);

			THEN("Delelgates are equal") {
				REQUIRE(equals);
			}
		}
	}

	GIVEN("Two delelgates to the same object but different methods") {
		auto foo1 = make_delegate(foo, &Foo::bar);
		auto foo2 = make_delegate(foo, &Foo::baz);

		WHEN("Delegates are compared") {
			bool equals = (foo1 == foo2);

			THEN("Delegates are not equal") {
				REQUIRE_FALSE(equals);
			}
		}
	}

	GIVEN("Two delegates to different objects") {
		auto first = make_delegate(foo, &Foo::bar);
		auto second = make_delegate(TestSingleton::GetInstance(), &TestSingleton::GetValue);
		auto third = make_delegate(TestSingleton::GetInstance(), &TestSingleton::GetValue);

		WHEN("Delegates are compared") {
			bool equals = (first == second);
			bool equals2 = (second == third);

			THEN("Delegates are not equal") {
				REQUIRE_FALSE(equals);
				REQUIRE(equals2);
			}
		}
	}
}