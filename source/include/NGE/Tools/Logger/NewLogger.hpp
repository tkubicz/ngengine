/* 
 * File:   NewLogger.hpp
 * Author: tku
 *
 * Created on 15 marca 2016, 02:55
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
#include "NGE/Tools/Logger/LogLevel.hpp"
#include "NGE/Tools/Logger/LogMessage.hpp"
#include "NGE/Tools/Logger/LoggerOutput.hpp"
#include "NGE/Tools/Logger/FileLoggerOutput.hpp"
#include "NGE/Tools/Logger/ConsoleLoggerOutput.hpp"

#define log_trace(format, ...) NGE::Tools::Logger::NewLogger::GetInstance().WriteLog(format, NGE::Tools::Logger::LogLevel::LOG_LEVEL::TRACE, __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__);
#define log_debug(format, ...) NGE::Tools::Logger::NewLogger::GetInstance().WriteLog(format, NGE::Tools::Logger::LogLevel::LOG_LEVEL::DEBUG, __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__);
#define log_info(format, ...) NGE::Tools::Logger::NewLogger::GetInstance().WriteLog(format, NGE::Tools::Logger::LogLevel::LOG_LEVEL::INFO, __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__);
#define log_warn(format, ...) NGE::Tools::Logger::NewLogger::GetInstance().WriteLog(format, NGE::Tools::Logger::LogLevel::LOG_LEVEL::WARN, __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__);
#define log_error(format, ...) NGE::Tools::Logger::NewLogger::GetInstance().WriteLog(format, NGE::Tools::Logger::LogLevel::LOG_LEVEL::ERROR, __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__);
#define log_critical(format, ...) NGE::Tools::NewLogger::GetInstance().WriteLog(format, NGE::Tools::Logger::LogLevel::LOG_LEVEL::CRITICAL_ERROR, __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__);

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
				LogLevel::LOG_LEVEL logLevel;

				std::map<std::string, LoggerOutput*> outputs;

			  private:

				/**
				 * Constructor that sets default values for fields. It is private,
				 * because this class is a singleton. Use GetInstance() to obtain
				 * instance of this class.
				 */
				NewLogger() {
					Initialise();
					InitialiseDefaultOutputs();
				}

				~NewLogger() {
					for (auto& kv : outputs) {
						if (kv.second != nullptr) {
							delete kv.second;
						}
					}
				}

				void InitialiseDefaultOutputs() {
					outputs.insert(std::pair<std::string, LoggerOutput*>("file", new FileLoggerOutput(logLevel, logFormat, dateFormat, flushAfter, false)));
					outputs.insert(std::pair<std::string, LoggerOutput*>("console", new ConsoleLoggerOutput(logLevel, logFormat, dateFormat, flushAfter, true)));
				}

			  public:

				static NewLogger& GetInstance() {
					static NewLogger instance;
					return instance;
				}

				void Initialise() {
					logLevel = LogLevel::LOG_LEVEL::TRACE;
					flushAfter = 20;
					logFormat = "{date} - {level} - {file} - {function}[{line}]: {log}";
					dateFormat = "%Y-%m-%d %H:%M:%S.%f";
				}

				template<typename... Args> void WriteLog(const std::string& format, LogLevel::LOG_LEVEL logLevel, const char* file, int line, const char* function, Args&&... arguments) {
					std::string message = fmt::format(format, std::forward<Args>(arguments)...);
					//					std::string formattedLog = fmt::format(logFormat, fmt::arg("date", Timing::GetInstance().GetCurrentTimeInFormat()),
					//							fmt::arg("level", LogLevel::LOG_LEVEL_NAME[static_cast<unsigned short> (logLevel)]), fmt::arg("file", file),
					//							fmt::arg("function", function), fmt::arg("line", line), fmt::arg("log", message));

					LogMessage msg(logLevel, file, function, line, message);

					for (auto& kv : outputs) {
						if (kv.second->IsEnabled()) {
							kv.second->GetQueue().Push(msg);
						}
					}
				}

				void Flush() {
					for (auto& kv : outputs) {
						if (kv.second->IsEnabled()) {
							kv.second->Flush();
						}
					}
				}

				LogLevel::LOG_LEVEL GetLogLevel() const {
					return logLevel;
				}

				void SetLogLevel(LogLevel::LOG_LEVEL logLevel) {
					this->logLevel = logLevel;
				}

				std::string GetLogFormat() const {
					return logFormat;
				}

				void SetLogFormat(std::string logFormat) {
					this->logFormat = logFormat;
				}

				std::string GetDateFormat() const {
					return dateFormat;
				}

				void SetDateFormat(std::string dateFormat) {
					this->dateFormat = dateFormat;
				}
			};
		}
	}
}

#endif /* NEWLOGGER_HPP */

