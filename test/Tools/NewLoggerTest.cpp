#include "catch.hpp"
#include <iostream>
#include <fstream>
#include <cstdio>
#include "NGE/Tools/Logger/NewLogger.hpp"
#include "NGE/Tools/Logger/Output/StreamLoggerOutput.hpp"
#include "NGE/Tools/Logger/Output/ConsoleLoggerOutput.hpp"
#include "NGE/Tools/Logger/Output/FileLoggerOutput.hpp"

namespace l = NGE::Tools::Logger;
namespace o = NGE::Tools::Logger::Output;

SCENARIO("Get logger outputs using subscript operator", "[logger]") {

	GIVEN("Logger with default configuration") {
		l::NewLogger& log = l::NewLogger::GetInstance();
		log.Initialise();

		WHEN("Subscript operator is used with identifier 'console'") {
			o::AbstractLoggerOutput* console = log["console"];

			THEN("AbstractLoggerOutput is not null") {
				REQUIRE(console != nullptr);
			}
		}

		WHEN("Subscript operator is used with identifier 'file'") {
			o::AbstractLoggerOutput* file = log["file"];

			THEN("AbstractLoggerOutput is not null") {
				REQUIRE(file != nullptr);
			}
		}

		WHEN("Subscript operator is used with identifier 'test' which doesn't exist") {
			o::AbstractLoggerOutput* test = log["test"];

			THEN("AbstractLoggerOutput is null") {
				REQUIRE(test == nullptr);
			}
		}

	}
}

template<typename T> void checkLoggerOutputDefaultConfiguration(T* output, const l::LogConfig& config) {
	REQUIRE(output->GetFlushAfter() == config.flushAfter);
	REQUIRE(output->IsAutoFlushEnabled() == config.autoFlushEnabled);
	REQUIRE(output->GetDateFormat() == config.dateFormat);
	REQUIRE(output->GetLogFormat() == config.logFormat);
	REQUIRE(output->GetLogLevel() == config.logLevel);
	REQUIRE(output->IsEnabled() == config.enabled);
}

SCENARIO("Get specified classes from AbstractLoggerOutputs", "[logger]") {

	GIVEN("Logger with default configuration") {
		l::NewLogger& log = l::NewLogger::GetInstance();
		log.Initialise();

		l::LogConfig config = log.GetLogConfig();

		WHEN("ConsoleLoggerOutput is requested") {
			o::ConsoleLoggerOutput* console = log.GetOutputs()["console"]->GetPtr<o::ConsoleLoggerOutput>();

			THEN("ConsoleLoggerOutput pointer is not null") {
				REQUIRE(console != nullptr);
			}

			THEN("ConsoleLoggerOutput has default configuration") {
				checkLoggerOutputDefaultConfiguration(console, config);
			}
		}

		WHEN("FileLoggerOutput is requested") {
			o::FileLoggerOutput* file = log.GetOutputs()["file"]->GetPtr<o::FileLoggerOutput>();

			THEN("FileLoggerOutput pointer is not null") {
				REQUIRE(file != nullptr);
			}

			THEN("FileLoggerOutput has default configuration") {
				checkLoggerOutputDefaultConfiguration(file, config);
				REQUIRE(file->GetFilePath() == "nge.log");
			}
		}

		WHEN("Class that does not derived from AbstractLoggerOutput") {
			l::LogConfig* test = log.GetOutputs()["test"]->GetPtr<l::LogConfig>();

			THEN("Should be null") {
				REQUIRE(test == nullptr);
			}
		}

		WHEN("ConsoleLoggerOutput is requested using subscription operator") {
			o::ConsoleLoggerOutput* console = log["console"]->GetPtr<o::ConsoleLoggerOutput>();

			THEN("ConsoleLoggerOutput pointer is not null") {
				REQUIRE(console != nullptr);
			}

			THEN("ConsoleLoggerOutput has default configuration") {
				checkLoggerOutputDefaultConfiguration(console, config);
			}
		}

		WHEN("ConsoleLoggerOutput is requested using subscription operator and requesting reference") {
			o::ConsoleLoggerOutput& console = log["console"]->GetRef<o::ConsoleLoggerOutput>();
			console.SetLogLevel(l::LogTypes::LOG_LEVEL::INFO);

			THEN("ConsoleLoggerOutput log level changed") {
				o::ConsoleLoggerOutput* consolePtr = log["console"]->GetPtr<o::ConsoleLoggerOutput>();
				REQUIRE(console.GetLogLevel() == consolePtr->GetLogLevel());
			}
		}

		WHEN("Reference is requested but class doesn't exist") {

			THEN("Exception std::bad_cast is required") {
				REQUIRE_THROWS_AS(log["console"]->GetRef<l::LogConfig>(), std::bad_cast);
			}
		}
	}
}

void checkDefaultConfiguration(const l::LogConfig& config) {
	REQUIRE(config.logLevel == l::LogTypes::LOG_LEVEL::TRACE);
	REQUIRE(config.enabled == true);
	REQUIRE(config.autoFlushEnabled == true);
	REQUIRE(config.flushAfter == 20);
	REQUIRE(config.logFormat == "[{date}][{level}][{shortFile}/{shortFunction}[{line}]] - {log}");
	REQUIRE(config.dateFormat == "%Y-%m-%d %H:%M:%S.%f");
}

SCENARIO("Check default configuration after initialising", "[logger]") {

	GIVEN("Logger instance") {
		l::NewLogger& log = l::NewLogger::GetInstance();

		WHEN("Logger is initialised") {
			log.Initialise();

			THEN("Logger has default configuration") {
				checkDefaultConfiguration(log.GetLogConfig());
			}

			THEN("Logger outputs have default configuration") {
				for (auto& output : log.GetOutputs()) {
					checkDefaultConfiguration(output.second->GetLogConfig());
				}
			}
		}
	}
}

SCENARIO("Check setting global configuration using setters", "[logger]") {

	GIVEN("Logger instance with default configuration and console and file outputs") {
		l::NewLogger& log = l::NewLogger::GetInstance();
		log.Initialise();
		o::ConsoleLoggerOutput* console = log["console"]->GetPtr<o::ConsoleLoggerOutput>();
		o::FileLoggerOutput* file = log["file"]->GetPtr<o::FileLoggerOutput>();
		REQUIRE(console != nullptr);
		REQUIRE(file != nullptr);

		WHEN("SetEnable(false) is invoked") {
			log.SetEnabled(false);

			THEN("Global logger should be disabled together with outputs") {
				REQUIRE(log.IsEnabled() == false);
				REQUIRE(console->IsEnabled() == false);
				REQUIRE(file->IsEnabled() == false);
			}
		}

		WHEN("SetLogLevel(INFO) is invoked") {
			log.SetLogLevel(l::LogTypes::LOG_LEVEL::INFO);

			THEN("Global logger should be on level info together with outputs") {
				REQUIRE(log.GetLogLevel() == l::LogTypes::LOG_LEVEL::INFO);
				REQUIRE(console->GetLogLevel() == l::LogTypes::LOG_LEVEL::INFO);
				REQUIRE(file->GetLogLevel() == l::LogTypes::LOG_LEVEL::INFO);
			}
		}

		WHEN("SetLogFormat({date} - {log}) is invoked") {
			const std::string format = "{date} - {log}";
			log.SetLogFormat(format);

			THEN("Global logger should have proper format together with outputs") {
				REQUIRE(log.GetLogFormat() == format);
				for (auto& kv : log.GetOutputs()) {
					REQUIRE(kv.second->GetLogFormat() == format);
				}
			}
		}

		WHEN("SetDateForm is invoked") {
			const std::string dateFormat = "dd/mm/yyyy";
			log.SetDateFormat(dateFormat);

			THEN("Global logger should have proper date format together with outputs") {
				REQUIRE(log.GetDateFormat() == dateFormat);
				for (auto& kv : log.GetOutputs()) {
					REQUIRE(kv.second->GetDateFormat() == dateFormat);
				}
			}
		}

		WHEN("SetAutoFlushEnabled is invoked") {
			log.SetAutoFlushEnabled(false);

			THEN("Global logger should have proper auto flush flag together with outputs") {
				REQUIRE(log.IsAutoFlushEnabled() == false);
				for (auto& kv : log.GetOutputs()) {
					REQUIRE(kv.second->IsAutoFlushEnabled() == false);
				}
			}
		}

		WHEN("SetFlushAfter is invoked") {
			const unsigned int flushAfter = 100;
			log.SetFlushAfter(flushAfter);

			THEN("Global logger should have proper flushAfter together with outputs") {
				REQUIRE(log.GetFlushAfter() == flushAfter);
				for (auto& kv : log.GetOutputs()) {
					REQUIRE(kv.second->GetFlushAfter() == flushAfter);
				}
			}
		}
	}
}

SCENARIO("Registering and unregistering logger output", "[logger]") {

	GIVEN("Logger instance with default configuration") {
		l::NewLogger& log = l::NewLogger::GetInstance();

		WHEN("New logger output is registered") {
			log.GetOutputs().insert(l::OutputPair("stream", new o::StreamLoggerOutput(log.GetLogConfig())));

			THEN("New output is available to log") {
				REQUIRE(log.GetOutputs().size() == 3);
				REQUIRE(log["stream"] != nullptr);
				REQUIRE(log["stream"]->GetPtr<o::StreamLoggerOutput>() != nullptr);
			}
		}

		WHEN("Logger output is unregistered") {
			REQUIRE(log.GetOutputs().size() == 3);
			log.GetOutputs().erase("stream");

			THEN("Unregistered output is not available anymore") {
				REQUIRE(log.GetOutputs().size() == 2);
				REQUIRE(log["stream"] == nullptr);
			}
		}
	}
}

void verifyStringStream(const std::stringstream& ss, const std::string& level, const std::string& logMsg) {
	std::string msg = ss.str();
	REQUIRE(msg.find(level) != std::string::npos);
	REQUIRE(msg.find(logMsg) != std::string::npos);
}

SCENARIO("Logging to console", "[logger]") {

	GIVEN("Logger with default console output enabled") {
		l::NewLogger& log = l::NewLogger::GetInstance();
		log.Initialise();
		log["file"]->SetEnabled(false);
		log["console"]->SetEnabled(true);

		std::stringstream ss;
		auto old = std::cout.rdbuf();
		std::cout.rdbuf(ss.rdbuf());

		WHEN("Trace log is written") {
			std::string logMsg = "trace message";
			log_trace("{}", logMsg);
			log.Flush();

			THEN("Message is written to console") {
				verifyStringStream(ss, "TRACE", logMsg);
			}
		}

		WHEN("Debug log is written") {
			std::string logMsg = "debug message";
			log_debug("{}", logMsg);
			log.Flush();

			THEN("Message is written to console") {
				verifyStringStream(ss, "DEBUG", logMsg);
			}
		}

		WHEN("Info log is written") {
			std::string logMsg = "info message";
			log_info("{}", logMsg);
			log.Flush();

			THEN("Message is written to console") {
				verifyStringStream(ss, "INFO", logMsg);
			}
		}

		WHEN("Warn log is written") {
			std::string logMsg = "Warn message";
			log_warn("{}", logMsg);
			log.Flush();

			THEN("Message is written to console") {
				verifyStringStream(ss, "WARN", logMsg);
			}
		}

		WHEN("Error log is written") {
			std::string logMsg = "error message";
			log_error("{}", logMsg);
			log.Flush();

			THEN("Message is written to console") {
				verifyStringStream(ss, "ERROR", logMsg);
			}
		}

		WHEN("Critical log is written") {
			std::string logMsg = "critical message";
			log_critical("{}", logMsg);
			log.Flush();

			THEN("Message is written to console") {
				verifyStringStream(ss, "CRITICAL", logMsg);
			}
		}

		std::cout.rdbuf(old);
	}
}

SCENARIO("Logging to file", "[logger]") {

	GIVEN("Logger with default file output enabled") {
		l::NewLogger& log = l::NewLogger::GetInstance();
		log.Initialise();
		log["file"]->SetEnabled(true);
		log["console"]->SetEnabled(false);
		const std::string fileName = "logging_to_file_test.log";
		(log["file"]->GetPtr<o::FileLoggerOutput>())->SetFilePath(fileName);

		WHEN("Logs are written and flushed") {

			std::array<std::string, 6> logMessages = {
				"trace log", "debug log", "info log",
				"warn log", "error log", "critical log"
			};

			log_trace(logMessages[0]);
			log_debug(logMessages[1]);
			log_info(logMessages[2]);
			log_warn(logMessages[3]);
			log_error(logMessages[4]);
			log_critical(logMessages[5]);
			log.Flush();

			THEN("Logs are in file") {
				std::ifstream file(fileName);
				REQUIRE(file.is_open());

				std::string line;
				unsigned short counter = 0;
				while (std::getline(file, line)) {
					REQUIRE(line.find(logMessages[counter++]) != std::string::npos);
				}
			}
		}

		std::remove(fileName.c_str());
	}
}

SCENARIO("Logging using different levels", "[logger]") {

	GIVEN("Logger with console logging enabled") {
		l::NewLogger& log = l::NewLogger::GetInstance();
		log.Initialise();
		log["file"]->SetEnabled(false);
		log["console"]->SetEnabled(true);

		std::stringstream ss;
		auto old = std::cout.rdbuf();
		std::cout.rdbuf(ss.rdbuf());

		WHEN("Log level is set to INFO") {
			log.SetLogLevel(l::LogTypes::LOG_LEVEL::INFO);

			THEN("TRACE and DEBUG should not be logged") {
				log_trace("trace msg");
				log_debug("debug msg");
				log.Flush();
				REQUIRE(ss.str().empty());
			}

			AND_THEN("INFO should be logged") {
				log_info("info msg");
				log.Flush();
				REQUIRE((!ss.str().empty() && ss.str().find("info msg") != std::string::npos));
			}

			AND_THEN("WARN should be logged") {
				log_warn("warn msg");
				log.Flush();
				REQUIRE((!ss.str().empty() && ss.str().find("warn msg") != std::string::npos));
			}

			AND_THEN("ERROR should be logged") {
				log_error("error msg");
				log.Flush();
				REQUIRE((!ss.str().empty() && ss.str().find("error msg") != std::string::npos));
			}

			AND_THEN("CRITICAL should be logged") {
				log_critical("critical msg");
				log.Flush();
				REQUIRE((!ss.str().empty() && ss.str().find("critical msg") != std::string::npos));
			}
		}

		std::cout.rdbuf(old);
	}
}

SCENARIO("Logging to different outputs using different levels", "[logger]") {

	GIVEN("Logger with console and stream logging enabled") {
		std::stringstream consoleStream, secondStream;
		l::NewLogger& log = l::NewLogger::GetInstance();
		log.Initialise();
		log["file"]->SetEnabled(false);
		log.GetOutputs().insert(l::OutputPair("stream", new o::StreamLoggerOutput(log.GetLogConfig())));
		log["stream"]->GetRef<o::StreamLoggerOutput>().SetOutputStream(&secondStream);

		auto old = std::cout.rdbuf();
		std::cout.rdbuf(consoleStream.rdbuf());

		// Set log levels.
		log["console"]->SetLogLevel(l::LogTypes::LOG_LEVEL::INFO);
		log["stream"]->SetLogLevel(l::LogTypes::LOG_LEVEL::WARN);

		WHEN("Logs are written") {

			WHEN("Trace log") {
				log_trace("trace msg");
				log.Flush();

				THEN("Both outputs should be empty") {
					REQUIRE(consoleStream.str().empty());
					REQUIRE(secondStream.str().empty());
				}
			}

			WHEN("Debug log") {
				log_debug("debug msg");
				log.Flush();

				THEN("Both outputs should be empty") {
					REQUIRE(consoleStream.str().empty());
					REQUIRE(secondStream.str().empty());
				}
			}

			WHEN("Info log") {
				log_info("info msg");
				log.Flush();

				THEN("Console output should contain the log") {
					REQUIRE(!consoleStream.str().empty());
				}

				AND_THEN("Stream output should be empty") {
					REQUIRE(secondStream.str().empty());
				}
			}

			WHEN("Warn log") {
				consoleStream.clear();
				secondStream.clear();
				log_warn("warn msg");
				log.Flush();

				THEN("Bot outputs should contains the log") {
					REQUIRE_FALSE(consoleStream.str().empty());
					REQUIRE_FALSE(secondStream.str().empty());
				}
			}
		}

		std::cout.rdbuf(old);
	}
}

SCENARIO("Automatic flush should work", "[logger]") {

	GIVEN("Logger with stream output enabled") {
		std::stringstream output;
		l::NewLogger& log = l::NewLogger::GetInstance();
		log.Initialise();
		log["file"]->SetEnabled(false);
		log["console"]->SetEnabled(false);
		log.GetOutputs().insert(l::OutputPair("stream", new o::StreamLoggerOutput(log.GetLogConfig())));
		log["stream"]->GetRef<o::StreamLoggerOutput>().SetOutputStream(&output);

		WHEN("Flush after is set to 1") {
			log.SetAutoFlushEnabled(true);
			log.SetFlushAfter(1);

			WHEN("Something is logged") {
				const std::string msg = "some debug message";
				log_debug(msg);

				THEN("Log should be in output without explicit flushing") {
					REQUIRE_FALSE(output.str().empty());
					REQUIRE(output.str().find(msg) != std::string::npos);
				}
			}
		}

		WHEN("Flush after is set to 5") {
			log.SetAutoFlushEnabled(true);
			log.SetFlushAfter(5);

			WHEN("4 messages are logged") {
				for (int i = 0; i < 4; ++i) {
					log_info("info msg #{}", i + 1);
				}

				THEN("Log output is empty") {
					REQUIRE(output.str().empty());
				}

				WHEN("5th message is logged") {
					log_info("info msg #5");

					THEN("Logs are automatically flushed") {
						REQUIRE_FALSE(output.str().empty());
					}
				}
			}
		}

		WHEN("AutoFlush is disabled") {
			log.SetAutoFlushEnabled(false);
			log.SetFlushAfter(1);

			WHEN("Few messages are logged") {
				for (int i = 0; i < 5; ++i) {
					log_warn("warn #{}", i + 1);
				}

				THEN("Log output should be empty") {
					REQUIRE(output.str().empty());
				}
			}
		}

		log.Initialise();
	}
}

SCENARIO("Load configuration from XML file", "[logger]") {

	GIVEN("Default Logger") {
		l::NewLogger& log = l::NewLogger::GetInstance();
		log.Initialise();

		WHEN("Configuration from XML is loaded") {
			REQUIRE(log.LoadXMLSettings("logger-configuration.xml"));

			THEN("Global logger should have configuration from XML file") {

			}
		}
	}
}

SCENARIO("Write logs from multiple threads", "[.][integration][logger]") {
    GIVEN("Logger with file output enabled") {
        l::NewLogger& log = l::NewLogger::GetInstance();
        log.Initialise();
        log["console"]->SetEnabled(false);
        
        WHEN("")
    }
}