#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "NGE/Tools/Timing.hpp"

using namespace NGE::Tools;

BOOST_AUTO_TEST_CASE(GetCurrentTimeInFormatTest) {
	Timing& timing = Timing::GetInstance();
	std::string currentTime = timing.GetCurrentTimeInFormat();
	std::cout << "Current time: " << currentTime << std::endl;
}