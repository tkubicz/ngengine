/* 
 * File:   NewLogger.hpp
 * Author: tku
 *
 * Created on 15 March 2016, 02:55
 */

#ifndef NEWLOGGER_HPP
#define NEWLOGGER_HPP

#include <list>
#include <map>
#include <fstream>
#include <mutex>
#include <cppformat/format.h>

#include "NGE/Core/Singleton.hpp"
#include "NGE/Core/XMLSettings.hpp"
#include "NGE/Core/ConcurrentQueue.hpp"
#include "NGE/Tools/Timing.hpp"
#include "NGE/Tools/Logger/LogTypes.hpp"
#include "NGE/Tools/Logger/LogConfig.hpp"
#include "NGE/Tools/Logger/LogMessage.hpp"
#include "NGE/Tools/Logger/Output/AbstractLoggerOutput.hpp"

#define _logger_message(logLevel, format, ...) \
    NGE::Tools::Logger::NewLogger::GetInstance().WriteLog(format, logLevel, __FILE__, __LINE__, __PRETTY_FUNCTION__,  ##__VA_ARGS__);

#define log_trace(format, ...) \
    _logger_message(NGE::Tools::Logger::LogTypes::LOG_LEVEL::TRACE, format, ##__VA_ARGS__);
#define log_debug(format, ...) \
    _logger_message(NGE::Tools::Logger::LogTypes::LOG_LEVEL::DEBUG, format, ##__VA_ARGS__);
#define log_info(format, ...) \
    _logger_message(NGE::Tools::Logger::LogTypes::LOG_LEVEL::INFO, format, ##__VA_ARGS__);
#define log_warn(format, ...) \
    _logger_message(NGE::Tools::Logger::LogTypes::LOG_LEVEL::WARN, format, ##__VA_ARGS__);
#define log_error(format, ...) \
    _logger_message(NGE::Tools::Logger::LogTypes::LOG_LEVEL::ERROR, format, ##__VA_ARGS__);
#define log_critical(format, ...) \
    _logger_message(NGE::Tools::Logger::LogTypes::LOG_LEVEL::CRITICAL, format, ##__VA_ARGS__);

namespace NGE {
    namespace Tools {
        namespace Logger {

            typedef std::map<std::string, Output::AbstractLoggerOutput*> OutputMap;
            typedef std::pair<std::string, Output::AbstractLoggerOutput*> OutputPair;

            class NewLogger : public NGE::Core::Singleton<NewLogger>, public NGE::Core::XMLSettings {
                friend class NGE::Core::Singleton<NewLogger>;

              private:

                LogConfig logConfig;

                OutputMap outputs;

                /**
                 * Constructor that sets default values for fields. It is private,
                 * because this class is a singleton. Use GetInstance() to obtain
                 * instance of this class.
                 */
                NewLogger();

                /**
                 * Private destructor. Because this class is a singleton, this destructor
                 * will be executed once, when the application will be destroyed.
                 */
                ~NewLogger();

                /**
                 * Initialisation of default logger outputs. By default console and
                 * file logger are available.
                 */
                void InitialiseDefaultOutputs();

                /**
                 * Helper method that checks if the message will be logged.
                 * @param logLevel Level of the log message.
                 * @param loggerLevel Level of the logger output.
                 * @return True if the loggerLevel is bigger or equal to logLevel.
                 */
                bool CheckLogMessageLevel(LogTypes::LOG_LEVEL logLevel, LogTypes::LOG_LEVEL loggerLevel);

                /**
                 * Clear all logger outputs. Delete pointers and clear the map.
                 */
                void ClearOutputs();

              public:

                /**
                 * Access logger outputs using subscript operator.
                 * @param name Identifier of the output. E.g. "file" or "console".
                 * @return Pointer to requested logger output. If not found, nullptr.
                 */
                Output::AbstractLoggerOutput* operator[](const std::string& name) {
                    auto findOutput = outputs.find(name);
                    if (findOutput == outputs.end()) {
                        log_warn("Could not find logger output: '{}'", name);

                        return nullptr;
                    }
                    return findOutput->second;
                }

                /**
                 * Initialise the logger and default outputs with default values.
                 * 
                 * This method initialises global logger options with default values,
                 * and all default outputs. If the logger was previously initialised,
                 * this method will rollback everything to default state.
                 */
                void Initialise();

                /**
                 * Log message. It is not recommended to use this method directly.
                 * Use defined macros instead.
                 * 
                 * @param format Format of the log message.
                 * @param logLevel Level of the log message.
                 * @param file Path to the file from where log was invoked.
                 * @param line Line of the code.
                 * @param function Name of the function.
                 * @param arguments Arguments matching format.
                 */
                template<typename... Args>
                void WriteLog(const std::string& format, LogTypes::LOG_LEVEL logLevel, const char* file, int line, const char* function, Args&&... arguments) {
                    std::string message = fmt::format(format, std::forward<Args>(arguments)...);
                    auto msg = std::make_shared<LogMessage>(logLevel, file, function, line, message);
                    for (auto& kv : outputs) {
                        if (kv.second->IsEnabled() && CheckLogMessageLevel(logLevel, kv.second->GetLogLevel())) {
                            kv.second->GetQueue().Push(msg);
                            kv.second->TryFlush();
                        }
                    }
                }

                /**
                 * Flush immediately all registered and enabled outputs. 
                 */
                void Flush();

                /**
                 * Check if the global logger is enabled.
                 * @return true if is enabled.
                 */
                bool IsEnabled() const;

                /**
                 * Enable or disable all registered outputs.
                 * @param enabled true if all registered outputs have to be enabled, or false to disable them.
                 */
                void SetEnabled(bool enabled);

                /**
                 * Get global logger log level.
                 * @return Current log level.
                 */
                LogTypes::LOG_LEVEL GetLogLevel() const;

                /**
                 * Set log level for all registered outputs.
                 * @param logLevel Desired log level.
                 */
                void SetLogLevel(LogTypes::LOG_LEVEL logLevel);

                std::string GetLogFormat() const;

                void SetLogFormat(const std::string& logFormat);

                std::string GetDateFormat() const;

                void SetDateFormat(const std::string& dateFormat);

                LogConfig GetLogConfig() const;

                void SetLogConfig(LogConfig logConfig);

                bool IsAutoFlushEnabled() const;

                void SetAutoFlushEnabled(bool autoFlushEnabled);

                unsigned int GetFlushAfter() const;

                void SetFlushAfter(unsigned int flushAfter);

                OutputMap& GetOutputs();

                bool LoadXMLSettings(const std::string& xmlFilePath) override;

                bool SaveXMLSettings(const std::string& xmlFilePath) override;
            };
        }
    }
}

#endif /* NEWLOGGER_HPP */

