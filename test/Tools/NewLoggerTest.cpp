#include "catch.hpp"
#include <iostream>
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
            o::ConsoleLoggerOutput* console = log.GetOutputs()["console"]->Get<o::ConsoleLoggerOutput>();

            THEN("ConsoleLoggerOutput pointer is not null") {
                REQUIRE(console != nullptr);
            }

            THEN("ConsoleLoggerOutput has default configuration") {
                checkLoggerOutputDefaultConfiguration(console, config);
            }
        }

        WHEN("FileLoggerOutput is requested") {
            o::FileLoggerOutput* file = log.GetOutputs()["file"]->Get<o::FileLoggerOutput>();

            THEN("FileLoggerOutput pointer is not null") {
                REQUIRE(file != nullptr);
            }

            THEN("FileLoggerOutput has default configuration") {
                checkLoggerOutputDefaultConfiguration(file, config);
                REQUIRE(file->GetFilePath() == "nge.log");
            }
        }

        WHEN("Class that does not derived from AbstractLoggerOutput") {
            l::LogConfig* test = log.GetOutputs()["test"]->Get<l::LogConfig>();

            THEN("Should be null") {
                REQUIRE(test == nullptr);
            }
        }

        WHEN("ConsoleLoggerOutput is requested using subscription operator") {
            o::ConsoleLoggerOutput* console = log["console"]->Get<o::ConsoleLoggerOutput>();

            THEN("ConsoleLoggerOutput pointer is not null") {
                REQUIRE(console != nullptr);
            }

            THEN("ConsoleLoggerOutput has default configuration") {
                checkLoggerOutputDefaultConfiguration(console, config);
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
        o::ConsoleLoggerOutput* console = log["console"]->Get<o::ConsoleLoggerOutput>();
        o::FileLoggerOutput* file = log["file"]->Get<o::FileLoggerOutput>();
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

SCENARIO("Registering new logger output") {

    GIVEN("Logger instance with default configuration") {
        l::NewLogger& log = l::NewLogger::GetInstance();

        WHEN("New logger output is registered") {
            log.GetOutputs().insert(l::OutputPair("stream", new o::StreamLoggerOutput(log.GetLogConfig())));

            THEN("New output is available to log") {
                REQUIRE(log.GetOutputs().size() == 3);
                REQUIRE(log["stream"] != nullptr);
                REQUIRE(log["stream"]->Get<o::StreamLoggerOutput>() != nullptr);
            }
        }
    }
}