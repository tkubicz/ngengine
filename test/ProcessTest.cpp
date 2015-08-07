#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <memory>
#include <boost/test/unit_test.hpp>
#include "NGE/Core/Process.hpp"
using namespace NGE::Core;

BOOST_AUTO_TEST_CASE(createProcessTest) {
    Process process;
	
	BOOST_CHECK_EQUAL(Process::UNINITIALIZED, process.GetState());
}
