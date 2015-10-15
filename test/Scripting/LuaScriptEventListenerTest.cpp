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

using namespace sel;
using namespace NGE::Scripting;
using namespace NGE::Events;

static int callbackFuncCounter = 0;

void callbackFunc(IEventDataPtr eventDataPtr) {
	callbackFuncCounter++;
}

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

BOOST_AUTO_TEST_CASE(CreateEventListenerTest) {
	LuaScriptEventListener* listener = new LuaScriptEventListener("test-id", 0x12345, callbackFunc);
	BOOST_CHECK(listener != nullptr);
}

BOOST_AUTO_TEST_CASE(TestScriptEventDelegate) {
	LuaScriptEventListener* listener = new LuaScriptEventListener("test-id", 0x12345, callbackFunc);
	TestEvent event;
	callbackFuncCounter = 0;

	listener->GetDelegate()(event.Copy());
	BOOST_CHECK_EQUAL(1, callbackFuncCounter);
}

BOOST_AUTO_TEST_CASE(DestroyEventListenerTest) {
	callbackFuncCounter = 0;
	EventManager manager("global-manager", true);
	NGE::Events::IEventManager* eventManager = NGE::Events::IEventManager::Get();
	BOOST_CHECK(eventManager != nullptr);

	std::string delegateId = "delegate-id";

	TestEvent event;
	manager.AddListener(delegateId, callbackFunc, event.GetEventType());
	manager.QueueEvent(event.Copy());
	manager.Update(20);
	BOOST_CHECK_EQUAL(1, callbackFuncCounter);
	manager.RemoveListener(delegateId, event.GetEventType());

	LuaScriptEventListener listener(delegateId, event.GetEventType(), callbackFunc);
	manager.AddListener(listener.GetDelegateId(), listener.GetDelegate(), event.GetEventType());

	manager.QueueEvent(event.Copy());
	manager.Update(100);
	//BOOST_CHECK_EQUAL(2, callbackFuncCounter);
}
