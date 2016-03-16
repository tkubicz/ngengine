#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "NGE/Tools/NewLogger.hpp"

using namespace NGE::Tools;

BOOST_AUTO_TEST_CASE(InitialTest) {
	test_log("{} {} {} {}", "ala", "ma", 2, "koty");
}