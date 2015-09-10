#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include "NGE/Events/EventManager.hpp"
#include "NGE/Core/Delegate.hpp"
#include "NGE/Events/BaseEventData.hpp"
using namespace NGE::Events;

class TestDelegateClass {
  public:

	void TestDelegateFunc1(const IEventDataPtr& event) {
		//std::cout << "TestDelegateFunc1 - " << "Event name: " << event->GetName() << std::endl;
	}

	void TestDelegateFunc2(const IEventDataPtr& event) {
		//std::cout << "TestDelegateFunc2 - " << "Event name: " << event->GetName() << std::endl;
	}

	void TestDelegateFunc3(const IEventDataPtr& event) {
		//std::cout << "TestDelegateFunc3 - " << "Event name: " << event->GetName() << std::endl;
	}

	void TestDelegateFunc4(const IEventDataPtr& event) {
		// Sleep for 20 ms
		std::chrono::milliseconds timespan(20);
		std::this_thread::sleep_for(timespan);
		//std::cout << "TestDelegateFunc4 - " << "Event name: " << event->GetName() << std::endl;
	}
};

class TestEvent : public BaseEventData {
  public:
	static const EventType eventType;

	TestEvent() {
	}

	virtual const EventType& GetEventType() const {
		return eventType;
	}

	virtual IEventDataPtr Copy() const {
		return IEventDataPtr(new TestEvent());
	}

	virtual const std::string GetName() const {
		return "TestEvent";
	}
};

const EventType TestEvent::eventType(0x1f357e69);

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

BOOST_AUTO_TEST_CASE(RemoveExistingDelegate) {
	EventManager eventManager("Event Manager", true);
	TestDelegateClass delegateClass;

	bool result = false;

	result = eventManager.AddListener("test delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1), 1);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.RemoveListener("test delegate1", 1);
	BOOST_CHECK_EQUAL(result, true);
}

BOOST_AUTO_TEST_CASE(RemoveNonexistingDelegate) {
	EventManager eventManager("Event Manager", true);
	TestDelegateClass delegateClass;

	bool result = false;

	result = eventManager.AddListener("test delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1), 1);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.RemoveListener("test delegate2", 1);
	BOOST_CHECK_EQUAL(result, false);
}

BOOST_AUTO_TEST_CASE(RemoveMultipleDelegates) {
	EventManager eventManager("Event Manager", true);
	TestDelegateClass delegateClass;

	bool result = false;

	result = eventManager.AddListener("test delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1), 1);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.AddListener("test delegate2", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc2), 1);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.AddListener("test delegate3", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc3), 1);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.RemoveListener("test delegate1", 1);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.RemoveListener("test delegate4", 1);
	BOOST_CHECK_EQUAL(result, false);

	result = eventManager.RemoveListener("test delegate2", 1);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.RemoveListener("test delegate3", 1);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.RemoveListener("test delegate3", 1);
	BOOST_CHECK_EQUAL(result, false);
}

BOOST_AUTO_TEST_CASE(RemoveDelegateForAnotherEventType) {
	EventManager eventManager("Event Manager", true);
	TestDelegateClass delegateClass;

	bool result = false;

	result = eventManager.AddListener("test delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1), 1);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.RemoveListener("test delegate1", 2);
	BOOST_CHECK_EQUAL(result, false);
}

BOOST_AUTO_TEST_CASE(TriggerEventForSingleDelegate) {
	EventManager eventManager("Event Manager", true);
	TestDelegateClass delegateClass;
	TestEvent event;
	bool result = false;

	result = eventManager.AddListener("test delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1), TestEvent::eventType);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.TriggerEvent(event.Copy());
	BOOST_CHECK_EQUAL(result, true);
}

BOOST_AUTO_TEST_CASE(TriggerEventForMultipleDelegate) {
	EventManager eventManager("Event Manager", true);
	TestDelegateClass delegateClass;
	TestEvent event;
	bool result = false;

	result = eventManager.AddListener("test delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1), TestEvent::eventType);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.AddListener("test delegate2", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc2), TestEvent::eventType);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.AddListener("test delegate3", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc3), TestEvent::eventType);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.TriggerEvent(event.Copy());
	BOOST_CHECK_EQUAL(result, true);
}

BOOST_AUTO_TEST_CASE(TriggerEventForNonexistingDelegate) {
	EventManager eventManager("Event Manager", true);
	TestEvent event;
	bool result = true;

	result = eventManager.TriggerEvent(event.Copy());
	BOOST_CHECK_EQUAL(result, false);
}

BOOST_AUTO_TEST_CASE(QueueNullEvent) {
	EventManager eventManager("Event Manager", true);
	bool result = true;
	result = eventManager.QueueEvent(nullptr);
	BOOST_CHECK_EQUAL(result, false);
}

BOOST_AUTO_TEST_CASE(QueueEventWithoutDelegate) {
	EventManager eventManager("Event Manager", true);
	TestEvent event;
	bool result = true;

	result = eventManager.QueueEvent(event.Copy());
	BOOST_CHECK_EQUAL(result, false);
}

BOOST_AUTO_TEST_CASE(QueueEvent) {
	EventManager eventManager("Event Manager", true);
	TestDelegateClass delegateClass;
	TestEvent event;
	bool result = true;

	result = eventManager.AddListener("test delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1), TestEvent::eventType);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.QueueEvent(event.Copy());
	BOOST_CHECK_EQUAL(result, true);
}

BOOST_AUTO_TEST_CASE(AbortSingleEvent) {
	EventManager eventManager("Event Manager", true);
	TestDelegateClass delegateClass;
	TestEvent event;
	bool result = false;

	result = eventManager.AddListener("test delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1), TestEvent::eventType);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.QueueEvent(event.Copy());
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.AbortEvent(TestEvent::eventType, false);
	BOOST_CHECK_EQUAL(result, true);
}

BOOST_AUTO_TEST_CASE(AbortAllEventsOfType) {
	EventManager eventManager("Event Manager", true);
	TestDelegateClass delegateClass;
	TestEvent event;
	bool result = false;

	result = eventManager.AddListener("test delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1), TestEvent::eventType);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.QueueEvent(event.Copy());
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.QueueEvent(event.Copy());
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.QueueEvent(event.Copy());
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.AbortEvent(TestEvent::eventType, true);
	BOOST_CHECK_EQUAL(result, true);
}

BOOST_AUTO_TEST_CASE(Update) {
	EventManager eventManager("Event Manager", true);
	TestDelegateClass delegateClass;
	TestEvent event;
	bool result = false;

	result = eventManager.AddListener("test delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1), TestEvent::eventType);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.AddListener("test delegate2", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc2), TestEvent::eventType);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.AddListener("test delegate3", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc3), TestEvent::eventType);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.QueueEvent(event.Copy());
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.Update(EventManager::INFINITE);
	BOOST_CHECK_EQUAL(result, true);
}

BOOST_AUTO_TEST_CASE(UpdateMultipleEvents) {
	EventManager eventManager("Event Manager", true);
	TestDelegateClass delegateClass;
	TestEvent event;
	bool result = false;

	result = eventManager.AddListener("test delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1), TestEvent::eventType);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.AddListener("test delegate2", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc2), TestEvent::eventType);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.AddListener("test delegate3", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc3), TestEvent::eventType);
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.QueueEvent(event.Copy());
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.QueueEvent(event.Copy());
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.QueueEvent(event.Copy());
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.QueueEvent(event.Copy());
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.QueueEvent(event.Copy());
	BOOST_CHECK_EQUAL(result, true);

	result = eventManager.Update(EventManager::INFINITE);
	BOOST_CHECK_EQUAL(result, true);
}

BOOST_AUTO_TEST_CASE(UpdateTimeout) {
	EventManager eventManager("Event Manager", true);
	TestDelegateClass delegateClass;
	TestEvent event;
	bool result = false;
	
	result = eventManager.AddListener("test delegate4", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc4), TestEvent::eventType);
	BOOST_CHECK_EQUAL(result, true);
	
	result = eventManager.QueueEvent(event.Copy());
	BOOST_CHECK_EQUAL(result, true);
	
	result = eventManager.QueueEvent(event.Copy());
	BOOST_CHECK_EQUAL(result, true);
	
	result = eventManager.Update(10);
	BOOST_CHECK_EQUAL(result, false);
}

BOOST_AUTO_TEST_CASE(UpdateTimeoutTwoIterations) {
	EventManager eventManager("Event Manager", true);
	TestDelegateClass delegateClass;
	TestEvent event;
	bool result = false;
	
	result = eventManager.AddListener("test delegate4", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc4), TestEvent::eventType);
	BOOST_CHECK_EQUAL(result, true);
	
	result = eventManager.QueueEvent(event.Copy());
	BOOST_CHECK_EQUAL(result, true);
	
	result = eventManager.QueueEvent(event.Copy());
	BOOST_CHECK_EQUAL(result, true);
	
	result = eventManager.Update(10);
	BOOST_CHECK_EQUAL(result, false);
	
	result = eventManager.Update(10);
	BOOST_CHECK_EQUAL(result, true);
}