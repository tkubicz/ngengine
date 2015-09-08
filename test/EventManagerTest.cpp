#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include "NGE/Events/EventManager.hpp"
using namespace NGE::Events;

BOOST_AUTO_TEST_CASE(CreateEventManager) {
	EventManager eventManager("Event Manager", true);
}
