#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>
#include <chrono>
#include <ratio>
#include <boost/test/unit_test.hpp>
#include "NGE/Tools/NewLogger.hpp"

using namespace NGE::Tools;

BOOST_AUTO_TEST_CASE(InitialTest) {
	test_trace_log("{} {} {} {}", "ala", "ma", 2, "koty");
	test_info_log("This is message: {:d}", 1);
	NewLogger::GetInstance().FlushToStdOut();
}

BOOST_AUTO_TEST_CASE(Test1000MessageLogs) {
	NewLogger& log = NewLogger::GetInstance();
	log.Initialise();

	auto startTime = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < 1000; ++i) {
		test_trace_log("message {} {} {}", 1, 2, 3);
	}

	auto stopTime = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double, std::milli> fp_ms = stopTime - startTime;
	std::cout << "Diff: " << fp_ms.count() << " ms" << std::endl;

	log.FlushToFile();
}

BOOST_AUTO_TEST_CASE(FlushToFileTest) {
	NewLogger& log = NewLogger::GetInstance();
	log.Initialise();

	for (int i = 0; i < 100; ++i) {
		test_info_log("This is a log message #{:d}", i);
	}

	//log.FlushToFile();
}