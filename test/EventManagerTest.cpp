#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include "NGE/Events/EventManager.hpp"
#include "NGE/Core/Delegate.hpp"
using namespace NGE::Events;

class TestDelegateClass {
  public:

	void TestDelegateFunc1(const IEventDataPtr& event) {
		std::cout << "TestDelegateFunc1 - " << "Event name: " << event->GetName() << std::endl;
	}

	void TestDelegateFunc2(const IEventDataPtr& event) {
		std::cout << "TestDelegateFunc2 - " << "Event name: " << event->GetName() << std::endl;
	}

	void TestDelegateFunc3(const IEventDataPtr& event) {
		std::cout << "TestDelegateFunc3 - " << "Event name: " << event->GetName() << std::endl;
	}
};

BOOST_AUTO_TEST_CASE(CreateEventManager) {
	EventManager eventManager("Event Manager", true);
}

BOOST_AUTO_TEST_CASE(AddNewListener) {
	EventManager eventManager("Event Manager", true);
	TestDelegateClass delegateClass;

	bool result = eventManager.AddListener("test delegate", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1), 12345);
	BOOST_CHECK_EQUAL(result, true);
}

BOOST_AUTO_TEST_CASE(AddTheSameListenerForTheSameEventType) {
	EventManager eventManager("Event Manager", true);
	TestDelegateClass delegateClass;

	bool result = eventManager.AddListener("test delegate", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1), 12345);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.AddListener("test delegate", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc2), 12345);
	BOOST_CHECK_EQUAL(result, false);
}

BOOST_AUTO_TEST_CASE(AddTheSameListenerForDiffrentEventType) {
	EventManager eventManager("Event Manager", true);
	TestDelegateClass delegateClass;

	bool result = eventManager.AddListener("test delegate", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1), 1);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.AddListener("test delegate", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1), 2);
	BOOST_CHECK_EQUAL(result, true);
}

BOOST_AUTO_TEST_CASE(AddMultipleListenersForTheSameEventType) {
	EventManager eventManager("Event Manager", true);
	TestDelegateClass delegateClass;

	bool result = false;

	result = eventManager.AddListener("test delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1), 1);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.AddListener("test delegate2", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc2), 1);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.AddListener("test delegate3", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc3), 1);
	BOOST_CHECK_EQUAL(result, true);
}