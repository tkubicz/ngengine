#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>
#include <chrono>
#include <ratio>
#include <boost/test/unit_test.hpp>
#include "NGE/Tools/Logger/NewLogger.hpp"
#include "NGE/Tools/Logger/Output/FileLoggerOutput.hpp"
#include "NGE/Tools/Logger/Output/ConsoleLoggerOutput.hpp"

using namespace NGE::Tools::Logger;
using namespace NGE::Tools::Logger::Output;

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

	FileLoggerOutput* fileLogger = dynamic_cast<FileLoggerOutput*> (log.GetOutputs()["file"]);
	fileLogger->SetFilePath("test_log_file.log");

	log_debug("This is some kind of a debug message");

	log.Flush();
}

BOOST_AUTO_TEST_CASE(TestLoggerLevels) {
	NewLogger& log = NewLogger::GetInstance();
	log.GetOutputs()["file"]->SetEnabled(true);
	log.GetOutputs()["console"]->SetEnabled(false);

	FileLoggerOutput* fileLogger = dynamic_cast<FileLoggerOutput*> (log.GetOutputs()["file"]);
	fileLogger->SetFilePath("test_logger_levels.log");

	log_trace("Log this msg");
	log_debug("Log this msg");

	log.GetOutputs()["file"]->SetLogLevel(LogTypes::LOG_LEVEL::INFO);

	log_trace("Do not log this msg");
	log_debug("Do not log this msg");
	log_info("Log this msg");
	log_warn("Log this msg");

	log.GetOutputs()["file"]->SetLogLevel(LogTypes::LOG_LEVEL::ERROR);

	log_trace("Do not log this msg");
	log_debug("Do not log this msg");
	log_info("Do not log this msg");
	log_warn("Do not log this msg");
	log_error("Log this msg");
	log_critical("Log this msg");

	log.Flush();
}

BOOST_AUTO_TEST_CASE(TestFlushing) {
	NewLogger& log = NewLogger::GetInstance();
	log.Initialise();
	log.GetOutputs()["file"]->SetEnabled(true);
	log.GetOutputs()["console"]->SetEnabled(true);

	FileLoggerOutput* fileLogger = dynamic_cast<FileLoggerOutput*> (log.GetOutputs()["file"]);
	fileLogger->SetFilePath("test_flushing.log");

	log_trace("Some trace message");
	log_info("Some info message");
	log_debug("Some debug message");
	log_critical("Some critical message");

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

BOOST_AUTO_TEST_CASE(WriteLogsFromMultipleThreads) {
	NewLogger& log = NewLogger::GetInstance();
	log.Initialise();
	log.GetOutputs()["file"]->SetEnabled(true);
	log.GetOutputs()["console"]->SetEnabled(false);

	FileLoggerOutput* fileLogger = dynamic_cast<FileLoggerOutput*> (log.GetOutputs()["file"]);
	fileLogger->SetFilePath("thread_log_file.log");

	const int numThreads = 50;
	std::array<std::thread, numThreads> threads;
	for (int i = 0; i < numThreads; ++i) {
		threads[i] = std::thread([]() {
			for (int i = 0; i < 10000; ++i) {
				log_info("{} / Info {}", std::this_thread::get_id(), i);
			}
		});
	}

	for (int i = 0; i < numThreads; ++i) {
		threads[i].join();
	}

	log.Flush();
}