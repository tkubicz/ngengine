#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "NGE/ThirdPart/selene.h"
#include "NGE/Core/Process.hpp"
#include "NGE/Tools/Logger.hpp"
#include "NGE/Events/BaseEventData.hpp"
#include "NGE/Scripting/LuaScriptEventListenerManager.hpp"
#include "NGE/Scripting/LuaScriptManager.hpp"

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

const EventType TestEvent::eventType(0x1f357e69);

void testCallbackFunc(IEventDataPtr event) {

}

BOOST_AUTO_TEST_CASE(AddListenerTest) {
	TestEvent event;
	std::function<void(IEventDataPtr) > func = testCallbackFunc;
	EventDelegate scriptDelegate("test", func);

	LuaScriptEventListener* listener = new LuaScriptEventListener(scriptDelegate, event.GetEventType());
	LuaScriptEventListenerManager manager;
	manager.AddListener(listener);

	BOOST_CHECK_EQUAL(1, manager.GetListenersSize());
}

BOOST_AUTO_TEST_CASE(DestroyListenerTest) {
	TestEvent event;
	std::function<void(IEventDataPtr) > func = testCallbackFunc;
	EventDelegate scriptDelegate("test", func);

	LuaScriptEventListener* listener = new LuaScriptEventListener(scriptDelegate, event.GetEventType());
	LuaScriptEventListenerManager manager;
	manager.AddListener(listener);

	BOOST_CHECK_EQUAL(1, manager.GetListenersSize());

	manager.DestroyListener(listener);

	BOOST_CHECK_EQUAL(0, manager.GetListenersSize());
}

BOOST_AUTO_TEST_CASE(DestructorTest) {
	TestEvent event;
	std::function<void(IEventDataPtr) > func = testCallbackFunc;
	EventDelegate scriptDelegate1("test1", func);
	EventDelegate scriptDelegate2("test2", func);
	EventDelegate scriptDelegate3("test3", func);

	LuaScriptEventListener* listener1 = new LuaScriptEventListener(scriptDelegate1, event.GetEventType());
	LuaScriptEventListener* listener2 = new LuaScriptEventListener(scriptDelegate2, event.GetEventType());
	LuaScriptEventListener* listener3 = new LuaScriptEventListener(scriptDelegate3, event.GetEventType());
	LuaScriptEventListenerManager* manager = new LuaScriptEventListenerManager();

	BOOST_CHECK(listener1 != nullptr);
	BOOST_CHECK(listener2 != nullptr);
	BOOST_CHECK(listener3 != nullptr);

	manager->AddListener(listener1);
	manager->AddListener(listener2);
	manager->AddListener(listener3);

	delete manager;
}