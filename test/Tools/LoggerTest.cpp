#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>
#include <chrono>
#include <ratio>
#include <boost/test/unit_test.hpp>
#include "NGE/Tools/Logger/NewLogger.hpp"

using namespace NGE::Tools::Logger;

BOOST_AUTO_TEST_CASE(InitialTest) {
    NewLogger& log = NewLogger::GetInstance();

    for (int i = 0; i < 10; ++i) {
        log_trace("{} {}: {}", "Trace", "message", 25);
        log_debug("Debug message: {} - {}", "test", 123.56);
        log_info("Info message: {:d}", 1);
        log_warn("Warn message: {}", "test");
        log_error("Error message");
        log_critical("Critical message: {} {} {}", 1, 2, "test");
    }
    log.Flush();
}

BOOST_AUTO_TEST_CASE(LogToFile) {
    NewLogger& log = NewLogger::GetInstance();
    log.GetOutputs()["file"]->SetEnabled(true);
    log.GetOutputs()["console"]->SetEnabled(true);

    FileLoggerOutput* fileLogger = dynamic_cast<FileLoggerOutput*>(log.GetOutputs()["file"]);
    fileLogger->SetFilePath("test_log_file.log");

    log_debug("This is some kind of a debug message");

    log.Flush();
}

//BOOST_AUTO_TEST_CASE(LogFormatTest) {
//	NewLogger& log = NewLogger::GetInstance();
//	log.SetLoggingtoStdOutEnabled(true);
//	log.SetLogFormat("{level} | {date} - {log}");
//
//	log_warn("This is a warning message: {}", "beware!");
//	log.SetLogFormat("{date} | {level} | {function} | {log}");
//	log_error("This is an error message: {:d}", 66);
//
//	log.Flush();
//}
//
//BOOST_AUTO_TEST_CASE(Test1000MessageLogs) {
//	NewLogger& log = NewLogger::GetInstance();
//	log.Initialise();
//	log.SetLoggingToFileEnabled(false);
//	log.SetLoggingtoStdOutEnabled(false);
//
//	auto startTime = std::chrono::high_resolution_clock::now();
//
//	for (int i = 0; i < 1000; ++i) {
//		log_trace("message {} {} {}", 1, 2, 3);
//	}
//
//	auto stopTime = std::chrono::high_resolution_clock::now();
//
//	std::chrono::duration<double, std::milli> fp_ms = stopTime - startTime;
//	std::cout << "Diff: " << fp_ms.count() << " ms" << std::endl;
//
//	log.Flush();
//}
//
//BOOST_AUTO_TEST_CASE(FlushToFileTest) {
//	NewLogger& log = NewLogger::GetInstance();
//	log.Initialise();
//	log.SetLoggingToFileEnabled(false);
//	log.SetLoggingtoStdOutEnabled(false);
//
//	for (int i = 0; i < 100; ++i) {
//		log_info("This is a log message #{:d}", i);
//	}
//
//	log.Flush();
//}
//
//BOOST_AUTO_TEST_CASE(WriteLogsFromMultipleThreads) {
//	NewLogger& log = NewLogger::GetInstance();
//	log.Initialise();
//	log.SetLoggingtoStdOutEnabled(false);
//	log.SetLoggingToFileEnabled(true);
//
//	const int numThreads = 10;
//	std::array<std::thread, numThreads> threads;
//	for (int i = 0; i < numThreads; ++i) {
//		threads[i] = std::thread([]() {
//			for (int i = 0; i < 100; ++i) {
//				log_info("Info {}", "msg");
//			}
//		});
//	}
//
//	for (int i = 0; i < numThreads; ++i) {
//		threads[i].join();
//	}
//
//	log.Flush();
//}