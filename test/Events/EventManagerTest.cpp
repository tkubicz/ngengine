#include "catch.hpp"
#include "NGE/Events/EventBus.hpp"
#include "NGE/Events/EventDelegate.hpp"
#include "NGE/Events/BaseEventData.hpp"
#include "NGE/Core/Delegate.hpp"
#include "NGE/Events/EventManager.hpp"
using namespace NGE::Events;

SCENARIO("EventManager creation", "[event-manager]") {

	WHEN("EventManager instance is get for the first time") {
		EventManager& manager = EventManager::GetInstance();

		THEN("We have an EventManager with global EventBus") {
			REQUIRE(manager.GetEventBusMap().size() == 0);
			REQUIRE(manager[manager.GLOBAL] != nullptr);
		}

		WHEN("Trying to get event bus that doesn't using subscript operator") {
			auto eventBus = manager["TEST"];

			THEN("EventBus should be null") {
				REQUIRE(eventBus == nullptr);
			}
		}
	}
}

SCENARIO("Creating and removing event buses", "[event-manager]") {

	GIVEN("EventManager instance") {
		EventManager& manager = EventManager::GetInstance();

		WHEN("New event bus is created") {
			const std::string name = "new-event-bus";
			auto newEventBus = manager.Create(name);

			THEN("New event bus is available in manager") {
				REQUIRE(newEventBus != nullptr);
				REQUIRE(manager.GetEventBusMap().size() == 1);
				REQUIRE(manager[name] == newEventBus);

				WHEN("Trying to create event bus with the same name second time") {
					auto secondEventBus = manager.Create(name);

					THEN("Create should return instance of the existing event bus") {
						REQUIRE(newEventBus == secondEventBus);
					}
				}
			}

			WHEN("Event bus is deleted") {
				bool deleteResult = manager.Delete(name);

				THEN("Event bus is no longer register in manager") {
					REQUIRE(deleteResult);
					REQUIRE(manager.GetEventBusMap().size() == 0);
					REQUIRE(manager[name] == nullptr);
				}
			}
		}

		WHEN("Trying to delete event bus that doesn't exist") {
			bool deleteResult = manager.Delete("test-event-bus");

			THEN("Delete method fails") {
				REQUIRE_FALSE(deleteResult);
			}
		}
	}
}

SCENARIO("Adding and removing listeners", "[event-manager]") {

	GIVEN("EventManager instance") {
		EventManager& manager = EventManager::GetInstance();

		GIVEN("Sample EventDelegate and EventType") {
			EventDelegate eventDelegate("test-delegate", [](IEventDataPtr ptr) {
			});
			EventType eventType = 0x887eadca;

			WHEN("Adding new listener to global event bus") {
				bool addResult = manager.AddListener(eventDelegate, eventType);

				THEN("Listener has been added") {
					REQUIRE(addResult);
				}

				WHEN("Removing listener from global event bus") {
					bool removeResult = manager.RemoveListener(eventDelegate, eventType);

					THEN("Listener has been removed") {
						REQUIRE(removeResult);
					}
				}
			}

			WHEN("Adding new listener to specified event bus that doesn't exist") {
				bool addResult = manager.AddListener("event-bus", eventDelegate, eventType);

				THEN("Adding listener fails, because event bus doesn't exist") {
					REQUIRE_FALSE(addResult);
				}
			}

			WHEN("Adding new listener to specified event bus") {
				manager.Create("event-bus");
				bool addResult = manager.AddListener("event-bus", eventDelegate, eventType);

				THEN("Listener has been added") {
					REQUIRE(addResult);

					WHEN("Removing listener from specified event bus") {
						bool removeResult = manager.RemoveListener("event-bus", eventDelegate, eventType);

						THEN("Listener has been removed") {
							REQUIRE(removeResult);
						}
					}
				}
			}
		}
	}
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