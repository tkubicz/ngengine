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

#include "NGE/Core/ConcurrentQueue.hpp"
#include "NGE/Tools/Timing.hpp"
#include "NGE/Tools/Logger/LogTypes.hpp"
#include "NGE/Tools/Logger/LogMessage.hpp"
#include "NGE/Tools/Logger/Output/LoggerOutput.hpp"

#define log_trace(format, ...) \
    NGE::Tools::Logger::NewLogger::GetInstance().WriteLog(format, NGE::Tools::Logger::LogTypes::LOG_LEVEL::TRACE, __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__);
#define log_debug(format, ...) \
    NGE::Tools::Logger::NewLogger::GetInstance().WriteLog(format, NGE::Tools::Logger::LogTypes::LOG_LEVEL::DEBUG, __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__);
#define log_info(format, ...) \
    NGE::Tools::Logger::NewLogger::GetInstance().WriteLog(format, NGE::Tools::Logger::LogTypes::LOG_LEVEL::INFO, __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__);
#define log_warn(format, ...) \
    NGE::Tools::Logger::NewLogger::GetInstance().WriteLog(format, NGE::Tools::Logger::LogTypes::LOG_LEVEL::WARN, __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__);
#define log_error(format, ...) \
    NGE::Tools::Logger::NewLogger::GetInstance().WriteLog(format, NGE::Tools::Logger::LogTypes::LOG_LEVEL::ERROR, __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__);
#define log_critical(format, ...) \
    NGE::Tools::Logger::NewLogger::GetInstance().WriteLog(format, NGE::Tools::Logger::LogTypes::LOG_LEVEL::CRITICAL, __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__);

namespace NGE {
	namespace Tools {
		namespace Logger {

			class NewLogger {
			  private:
				/**
				 * Format of the logged message
				 */
				std::string logFormat;

				/**
				 * Format of the date used in logger.
				 */
				std::string dateFormat;

				/**
				 * Default maximum size of the log queue.
				 */
				unsigned int flushAfter;

				/**
				 * Current logging level.
				 */
				LogTypes::LOG_LEVEL logLevel;

				std::map<std::string, Output::LoggerOutput*> outputs;

			  private:

				/**
				 * Constructor that sets default values for fields. It is private,
				 * because this class is a singleton. Use GetInstance() to obtain
				 * instance of this class.
				 */
				NewLogger();

				~NewLogger();

				void InitialiseDefaultOutputs();

				bool CheckLogMessageLevel(LogTypes::LOG_LEVEL logLevel, LogTypes::LOG_LEVEL loggerLevel);
				
				void ClearOutputs();

			  public:

				static NewLogger& GetInstance();

				/**
				 * Initialise the logger and default outputs with default values.
				 * 
				 * This method initialises global logger options with default values,
				 * and all default outputs.
				 */
				void Initialise();

				template<typename... Args>
				void WriteLog(const std::string& format, LogTypes::LOG_LEVEL logLevel, const char* file, int line, const char* function, Args&&... arguments) {
					std::string message = fmt::format(format, std::forward<Args>(arguments)...);
					LogMessage msg(logLevel, file, function, line, message);

					for (auto& kv : outputs) {
						if (kv.second->IsEnabled() && CheckLogMessageLevel(logLevel, kv.second->GetLogLevel())) {
							kv.second->GetQueue().Push(msg);
						}
					}
				}

				void Flush();

				LogTypes::LOG_LEVEL GetLogLevel() const;

				void SetLogLevel(LogTypes::LOG_LEVEL logLevel);

				std::string GetLogFormat() const;

				void SetLogFormat(const std::string& logFormat);

				std::string GetDateFormat() const;

				void SetDateFormat(const std::string& dateFormat);

				std::map<std::string, Output::LoggerOutput*>& GetOutputs();
			};
		}
	}
}

#endif /* NEWLOGGER_HPP */

