#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "NGE/ThirdPart/selene.h"
#include "NGE/Events/IEventData.hpp"
#include "NGE/Events/BaseEventData.hpp"
#include "NGE/Events/IEventManager.hpp"
#include "NGE/Scripting/LuaScriptEventListener.hpp"
#include "NGE/Events/EventManager.hpp"
#include "NGE/Scripting/LuaScriptEvent.hpp"

using namespace sel;
using namespace NGE::Scripting;
using namespace NGE::Events;

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

class OtherEvent : public BaseEventData {
  public:
	static const EventType eventType;

	int value;

	OtherEvent(int value) : value(value) {
	}

	virtual const EventType& GetEventType() const {
		return eventType;
	}

	virtual IEventDataPtr Copy() const {
		return IEventDataPtr(new OtherEvent(value));
	}

	virtual const std::string GetName() const {
		return "OtherEvent";
	}
};

static int callbackFuncCounter = 0;

void callbackFunc(IEventDataPtr eventDataPtr) {
	callbackFuncCounter++;
}

void otherEventCallback(IEventDataPtr eventDataPtr) {
	std::cout << std::static_pointer_cast<OtherEvent>(eventDataPtr)->value << std::endl;
}

const EventType TestEvent::eventType(0x1f357e69);
const EventType OtherEvent::eventType(0x1f55845);

BOOST_AUTO_TEST_CASE(CreateEventListenerTest) {
	EventDelegate delegate("test-delegate", callbackFunc);
	LuaScriptEventListener* listener = new LuaScriptEventListener(delegate, 0x12345);
	BOOST_CHECK(listener != nullptr);
}

BOOST_AUTO_TEST_CASE(TestScriptEventDelegate) {
	EventDelegate delegate("test-delegate", callbackFunc);
	LuaScriptEventListener* listener = new LuaScriptEventListener(delegate, 0x12345);
	TestEvent event;
	callbackFuncCounter = 0;

	listener->GetDelegate()(event.Copy());
	BOOST_CHECK_EQUAL(1, callbackFuncCounter);
}

BOOST_AUTO_TEST_CASE(AddListenerToEventManager) {
	EventManager manager("manager", false);
	OtherEvent event(10);
	EventDelegate delegate1("callbackFunc", otherEventCallback);
	EventDelegate delegate2("anotherFunc", otherEventCallback);
	EventDelegate delegate3("otherFunc", otherEventCallback);

	LuaScriptEventListener listener(delegate1, event.GetEventType());

	manager.AddListener(listener.GetEventDelegate(), event.GetEventType());
	manager.TriggerEvent(event.Copy());

	LuaScriptEventListener listener2(delegate2, event.GetEventType());
	manager.AddListener(listener2.GetEventDelegate(), event.GetEventType());
	manager.TriggerEvent(event.Copy());

	manager.RemoveListener(delegate2, event.GetEventType());
	manager.RemoveListener(delegate1, event.GetEventType());

	manager.AddListener(delegate3, event.GetEventType());
	manager.TriggerEvent(std::make_shared<OtherEvent>(20));
}

BOOST_AUTO_TEST_CASE(DestroyEventListenerTest) {
	callbackFuncCounter = 0;
	EventManager manager("global-manager", false);
	EventDelegate delegate("delegate-id", callbackFunc);
	TestEvent event;

	manager.AddListener(delegate, event.GetEventType());
	manager.TriggerEvent(event.Copy());
	BOOST_CHECK_EQUAL(1, callbackFuncCounter);
	manager.RemoveListener(delegate, event.GetEventType());

	LuaScriptEventListener listener(delegate, event.GetEventType());
	manager.AddListener(listener.GetEventDelegate(), event.GetEventType());
	manager.TriggerEvent(event.Copy());
	BOOST_CHECK_EQUAL(2, callbackFuncCounter);
}
