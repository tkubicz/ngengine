#include "catch.hpp"
#include <boost/test/unit_test.hpp>
#include "NGE/Events/EventManager.hpp"
#include "NGE/Events/EventDelegate.hpp"
#include "NGE/Events/BaseEventData.hpp"
#include "NGE/Core/Delegate.hpp"
using namespace NGE::Events;

SCENARIO("Create event manager", "[event-manager]") {

	WHEN("Local Event managers are created") {
		std::unique_ptr<IEventManager> manager1(new EventManager("manager-1", false));
		std::unique_ptr<IEventManager> manager2(new EventManager("manager-2", false));

		THEN("Event managers exist") {
			REQUIRE(manager1 != nullptr);
			REQUIRE(manager2 != nullptr);
		}

		AND_THEN("Global event manager doesn't exist") {
			REQUIRE(IEventManager::Get() == nullptr);
		}
	}

	WHEN("Global event manager is created") {
		EventManager manager1("global-event-manager", true);

		THEN("IEventManager::Get() should return the same object") {
			REQUIRE(&manager1 == IEventManager::Get());
		}
	}

//	WHEN("Two global managers are created") {
//		EventManager* manager1 = new EventManager("first-event-manager", true);
//		EventManager* manager2 = new EventManager("second-event-manager", true);
//
//		THEN("Event manager created last should be global") {
//			REQUIRE_FALSE(manager1 == IEventManager::Get());
//			REQUIRE(manager2 == IEventManager::Get());
//
//			if (manager1 != nullptr) delete manager1;
//			if (manager2 != nullptr) delete manager2;
//		}
//	}
}

//class TestDelegateClass {
//  public:
//
//	void TestDelegateFunc1(const IEventDataPtr& event) {
//		//std::cout << "TestDelegateFunc1 - " << "Event name: " << event->GetName() << std::endl;
//	}
//
//	void TestDelegateFunc2(const IEventDataPtr& event) {
//		//std::cout << "TestDelegateFunc2 - " << "Event name: " << event->GetName() << std::endl;
//	}
//
//	void TestDelegateFunc3(const IEventDataPtr& event) {
//		//std::cout << "TestDelegateFunc3 - " << "Event name: " << event->GetName() << std::endl;
//	}
//
//	void TestDelegateFunc4(const IEventDataPtr& event) {
//		// Sleep for 20 ms
//		std::chrono::milliseconds timespan(20);
//		std::this_thread::sleep_for(timespan);
//		//std::cout << "TestDelegateFunc4 - " << "Event name: " << event->GetName() << std::endl;
//	}
//};
//
//class TestEvent : public BaseEventData {
//  public:
//	static const EventType eventType;
//
//	TestEvent() {
//	}
//
//	virtual const EventType& GetEventType() const {
//		return eventType;
//	}
//
//	virtual IEventDataPtr Copy() const {
//		return IEventDataPtr(new TestEvent());
//	}
//
//	virtual const std::string GetName() const {
//		return "TestEvent";
//	}
//};
//
//const EventType TestEvent::eventType(0x1f357e69);
//
//BOOST_AUTO_TEST_CASE(CreateEventManager) {
//	EventManager eventManager("Event Manager", true);
//}
//
//BOOST_AUTO_TEST_CASE(CreateGlobalEventManager) {
//	EventManager eventManager("EventManager", true);
//	IEventManager* globalEventManager = IEventManager::Get();
//	BOOST_CHECK_EQUAL(&eventManager, globalEventManager);
//}
//
//BOOST_AUTO_TEST_CASE(AddNewListener) {
//	EventManager eventManager("Event Manager", true);
//	TestDelegateClass delegateClass;
//	EventDelegate eventDelegate("test-delegate", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1));
//
//	bool result = eventManager.AddListener(eventDelegate, 12345);
//	BOOST_CHECK_EQUAL(result, true);
//}
//
//BOOST_AUTO_TEST_CASE(AddTheSameListenerForTheSameEventType) {
//	EventManager eventManager("Event Manager", true);
//	TestDelegateClass delegateClass;
//	EventDelegate delegate1("test-delegate", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1));
//	EventDelegate delegate2("test-delegate", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc2));
//
//	bool result = eventManager.AddListener(delegate1, 12345);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.AddListener(delegate2, 12345);
//	BOOST_CHECK_EQUAL(result, false);
//}
//
//BOOST_AUTO_TEST_CASE(AddTheSameListenerForDiffrentEventType) {
//	EventManager eventManager("Event Manager", true);
//	TestDelegateClass delegateClass;
//	EventDelegate eventDelegate("test-delegate", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1));
//
//	bool result = eventManager.AddListener(eventDelegate, 1);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.AddListener(eventDelegate, 2);
//	BOOST_CHECK_EQUAL(result, true);
//}
//
//BOOST_AUTO_TEST_CASE(AddMultipleListenersForTheSameEventType) {
//	EventManager eventManager("Event Manager", true);
//	TestDelegateClass delegateClass;
//	EventDelegate delegate1("test-delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1));
//	EventDelegate delegate2("test-delegate2", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc2));
//	EventDelegate delegate3("test-delegate3", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc3));
//
//	bool result = false;
//
//	result = eventManager.AddListener(delegate1, 1);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.AddListener(delegate2, 1);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.AddListener(delegate3, 1);
//	BOOST_CHECK_EQUAL(result, true);
//}
//
//BOOST_AUTO_TEST_CASE(RemoveExistingDelegate) {
//	EventManager eventManager("Event Manager", true);
//	TestDelegateClass delegateClass;
//	EventDelegate eventDelegate("test-delegate", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1));
//
//	bool result = false;
//
//	result = eventManager.AddListener(eventDelegate, 1);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.RemoveListener(eventDelegate, 1);
//	BOOST_CHECK_EQUAL(result, true);
//}
//
//BOOST_AUTO_TEST_CASE(RemoveNonexistingDelegate) {
//	EventManager eventManager("Event Manager", true);
//	TestDelegateClass delegateClass;
//	EventDelegate eventDelegate1("test-delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1));
//	EventDelegate eventDelegate2("test-delegate2", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1));
//
//	bool result = false;
//
//	result = eventManager.AddListener(eventDelegate1, 1);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.RemoveListener(eventDelegate2, 1);
//	BOOST_CHECK_EQUAL(result, false);
//}
//
//BOOST_AUTO_TEST_CASE(RemoveMultipleDelegates) {
//	EventManager eventManager("Event Manager", true);
//	TestDelegateClass delegateClass;
//	EventDelegate delegate1("test-delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1));
//	EventDelegate delegate2("test-delegate2", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc2));
//	EventDelegate delegate3("test-delegate3", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc3));
//	EventDelegate delegate4("test-delegate4", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc4));
//
//	bool result = false;
//
//	result = eventManager.AddListener(delegate1, 1);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.AddListener(delegate2, 1);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.AddListener(delegate3, 1);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.RemoveListener(delegate1, 1);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.RemoveListener(delegate4, 1);
//	BOOST_CHECK_EQUAL(result, false);
//
//	result = eventManager.RemoveListener(delegate2, 1);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.RemoveListener(delegate3, 1);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.RemoveListener(delegate3, 1);
//	BOOST_CHECK_EQUAL(result, false);
//}
//
//BOOST_AUTO_TEST_CASE(RemoveDelegateForAnotherEventType) {
//	EventManager eventManager("Event Manager", true);
//	TestDelegateClass delegateClass;
//	EventDelegate delegate1("test-delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1));
//
//	bool result = false;
//
//	result = eventManager.AddListener(delegate1, 1);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.RemoveListener(delegate1, 2);
//	BOOST_CHECK_EQUAL(result, false);
//}
//
//BOOST_AUTO_TEST_CASE(TriggerEventForSingleDelegate) {
//	EventManager eventManager("Event Manager", true);
//	TestDelegateClass delegateClass;
//	TestEvent event;
//	EventDelegate delegate1("test-delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1));
//	bool result = false;
//
//	result = eventManager.AddListener(delegate1, TestEvent::eventType);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.TriggerEvent(event.Copy());
//	BOOST_CHECK_EQUAL(result, true);
//}
//
//BOOST_AUTO_TEST_CASE(TriggerEventForMultipleDelegate) {
//	EventManager eventManager("Event Manager", true);
//	TestDelegateClass delegateClass;
//	TestEvent event;
//	EventDelegate delegate1("test-delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1));
//	EventDelegate delegate2("test-delegate2", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc2));
//	EventDelegate delegate3("test-delegate3", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc3));
//
//	bool result = false;
//
//	result = eventManager.AddListener(delegate1, TestEvent::eventType);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.AddListener(delegate2, TestEvent::eventType);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.AddListener(delegate3, TestEvent::eventType);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.TriggerEvent(event.Copy());
//	BOOST_CHECK_EQUAL(result, true);
//}
//
//BOOST_AUTO_TEST_CASE(TriggerEventForNonexistingDelegate) {
//	EventManager eventManager("Event Manager", true);
//	TestEvent event;
//	bool result = true;
//
//	result = eventManager.TriggerEvent(event.Copy());
//	BOOST_CHECK_EQUAL(result, false);
//}
//
//BOOST_AUTO_TEST_CASE(QueueNullEvent) {
//	EventManager eventManager("Event Manager", true);
//	bool result = true;
//	result = eventManager.QueueEvent(nullptr);
//	BOOST_CHECK_EQUAL(result, false);
//}
//
//BOOST_AUTO_TEST_CASE(QueueEventWithoutDelegate) {
//	EventManager eventManager("Event Manager", true);
//	TestEvent event;
//	bool result = true;
//
//	result = eventManager.QueueEvent(event.Copy());
//	BOOST_CHECK_EQUAL(result, false);
//}
//
//BOOST_AUTO_TEST_CASE(QueueEvent) {
//	EventManager eventManager("Event Manager", true);
//	TestDelegateClass delegateClass;
//	TestEvent event;
//	EventDelegate delegate1("test-delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1));
//	bool result = true;
//
//	result = eventManager.AddListener(delegate1, TestEvent::eventType);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.QueueEvent(event.Copy());
//	BOOST_CHECK_EQUAL(result, true);
//}
//
//BOOST_AUTO_TEST_CASE(AbortSingleEvent) {
//	EventManager eventManager("Event Manager", true);
//	TestDelegateClass delegateClass;
//	TestEvent event;
//	EventDelegate delegate1("test-delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1));
//	bool result = false;
//
//	result = eventManager.AddListener(delegate1, TestEvent::eventType);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.QueueEvent(event.Copy());
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.AbortEvent(TestEvent::eventType, false);
//	BOOST_CHECK_EQUAL(result, true);
//}
//
//BOOST_AUTO_TEST_CASE(AbortAllEventsOfType) {
//	EventManager eventManager("Event Manager", true);
//	TestDelegateClass delegateClass;
//	TestEvent event;
//	EventDelegate delegate1("test-delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1));
//
//	bool result = false;
//
//	result = eventManager.AddListener(delegate1, TestEvent::eventType);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.QueueEvent(event.Copy());
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.QueueEvent(event.Copy());
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.QueueEvent(event.Copy());
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.AbortEvent(TestEvent::eventType, true);
//	BOOST_CHECK_EQUAL(result, true);
//}
//
//BOOST_AUTO_TEST_CASE(Update) {
//	EventManager eventManager("Event Manager", true);
//	TestDelegateClass delegateClass;
//	TestEvent event;
//	EventDelegate delegate1("test-delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1));
//	EventDelegate delegate2("test-delegate2", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc2));
//	EventDelegate delegate3("test-delegate3", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc3));
//	bool result = false;
//
//	result = eventManager.AddListener(delegate1, TestEvent::eventType);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.AddListener(delegate2, TestEvent::eventType);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.AddListener(delegate3, TestEvent::eventType);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.QueueEvent(event.Copy());
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.Update(EventManager::INFINITE);
//	BOOST_CHECK_EQUAL(result, true);
//}
//
//BOOST_AUTO_TEST_CASE(UpdateMultipleEvents) {
//	EventManager eventManager("Event Manager", true);
//	TestDelegateClass delegateClass;
//	TestEvent event;
//	EventDelegate delegate1("test-delegate1", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc1));
//	EventDelegate delegate2("test-delegate2", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc2));
//	EventDelegate delegate3("test-delegate3", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc3));
//	bool result = false;
//
//	result = eventManager.AddListener(delegate1, TestEvent::eventType);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.AddListener(delegate2, TestEvent::eventType);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.AddListener(delegate3, TestEvent::eventType);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.QueueEvent(event.Copy());
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.QueueEvent(event.Copy());
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.QueueEvent(event.Copy());
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.QueueEvent(event.Copy());
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.QueueEvent(event.Copy());
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.Update(EventManager::INFINITE);
//	BOOST_CHECK_EQUAL(result, true);
//}
//
//BOOST_AUTO_TEST_CASE(UpdateTimeout) {
//	EventManager eventManager("Event Manager", true);
//	TestDelegateClass delegateClass;
//	TestEvent event;
//	EventDelegate delegate4("test-delegate4", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc4));
//	bool result = false;
//
//	result = eventManager.AddListener(delegate4, TestEvent::eventType);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.QueueEvent(event.Copy());
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.QueueEvent(event.Copy());
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.Update(10);
//	BOOST_CHECK_EQUAL(result, false);
//}
//
//BOOST_AUTO_TEST_CASE(UpdateTimeoutTwoIterations) {
//	EventManager eventManager("Event Manager", true);
//	TestDelegateClass delegateClass;
//	TestEvent event;
//	EventDelegate delegate4("test-delegate4", NGE::Core::make_delegate(delegateClass, &TestDelegateClass::TestDelegateFunc4));
//	bool result = false;
//
//	result = eventManager.AddListener(delegate4, TestEvent::eventType);
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.QueueEvent(event.Copy());
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.QueueEvent(event.Copy());
//	BOOST_CHECK_EQUAL(result, true);
//
//	result = eventManager.Update(10);
//	BOOST_CHECK_EQUAL(result, false);
//
//	result = eventManager.Update(10);
//	BOOST_CHECK_EQUAL(result, true);
//}