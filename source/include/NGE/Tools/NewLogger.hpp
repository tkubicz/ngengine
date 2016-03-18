/* 
 * File:   NewLogger.hpp
 * Author: tku
 *
 * Created on 15 marca 2016, 02:55
 */

#ifndef NEWLOGGER_HPP
#define NEWLOGGER_HPP

#include <list>
#include <fstream>
#include <mutex>
#include <cppformat/format.h>
#include "NGE/Core/ConcurrentQueue.hpp"
#include "NGE/Tools/Timing.hpp"

#define log_trace(format, ...) NGE::Tools::NewLogger::GetInstance().WriteLog(format, NGE::Tools::NewLogger::LOG_LEVEL::TRACE, __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__);
#define log_debug(format, ...) NGE::Tools::NewLogger::GetInstance().WriteLog(format, NGE::Tools::NewLogger::LOG_LEVEL::DEBUG, __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__);
#define log_info(format, ...) NGE::Tools::NewLogger::GetInstance().WriteLog(format, NGE::Tools::NewLogger::LOG_LEVEL::INFO, __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__);
#define log_warn(format, ...) NGE::Tools::NewLogger::GetInstance().WriteLog(format, NGE::Tools::NewLogger::LOG_LEVEL::WARN, __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__);
#define log_error(format, ...) NGE::Tools::NewLogger::GetInstance().WriteLog(format, NGE::Tools::NewLogger::LOG_LEVEL::ERROR, __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__);
#define log_critical(format, ...) NGE::Tools::NewLogger::GetInstance().WriteLog(format, NGE::Tools::NewLogger::LOG_LEVEL::CRITICAL_ERROR, __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__);

namespace NGE {
	namespace Tools {

		class NewLogger {
		  public:

			enum class LOG_LEVEL : unsigned short {
				TRACE = 0, DEBUG = 1, INFO = 2, WARN = 3, ERROR = 4, CRITICAL_ERROR = 5
			};

		  private:

			static const std::array<std::string, 6> logLevelNames;

			/**
			 * Holds currently accumulated logs.
			 */
			NGE::Core::ConcurrentQueue<std::string> logQueue;


			/**
			 * Strings drained from logQueue. Cleared after flush.
			 */
			NGE::Core::ConcurrentQueue<std::string> flushQueue;

			/**
			 * Format of the logged message
			 */
			std::string logFormat;

			/**
			 * Format of the date used in logger.
			 */
			std::string dateFormat;

			/**
			 * Path to the log file.
			 */
			std::string logFilePath;

			/**
			 * Flag that indicates that we are logging to the file.
			 */
			bool loggingToFileEnabled;

			/**
			 * Flag that indicates that we are logging to standard output.
			 */
			bool loggingtoStdOutEnabled;

			unsigned int flushSize;

			/**
			 * Current logging level.
			 */
			LOG_LEVEL logLevel;

		  private:

			/**
			 * Constructor that sets default values for fields. It is private,
			 * because this class is a singleton. Use GetInstance() to obtain
			 * instance of this class.
			 */
			NewLogger() {
				Initialise();
			}

			void FlushToStdOut() {
				while (logQueue.Size() > 0) {
					std::string logMessage;
					if (logQueue.TryPop(logMessage)) {
						std::cout << logMessage << "\n";
					}
				}
			}

			void FlushToFile() {
				std::ofstream logFile(logFilePath, std::ios::app);
				while (logQueue.Size() > 0) {
					std::string logMessage;
					if (logQueue.TryPop(logMessage)) {
						logFile << logMessage << "\n";
					}
				}
			}

		  public:

			static NewLogger& GetInstance() {
				static NewLogger instance;
				return instance;
			}

			void Initialise() {
				logLevel = LOG_LEVEL::TRACE;
				loggingToFileEnabled = false;
				loggingtoStdOutEnabled = true;
				flushSize = 20;
				logFilePath = "nge.log";
				logFormat = "{date} - {level} - {file} - {function}[{line}]: {log}";

				if (!logQueue.Empty()) {
					logQueue.Clear();
				}
			}

			template<typename... Args> void WriteLog(const std::string& format, LOG_LEVEL logLevel, const char* file, int line, const char* function, Args&&... arguments) {
				std::string message = fmt::format(format, std::forward<Args>(arguments)...);
				std::string formattedLog = fmt::format(logFormat, fmt::arg("date", Timing::GetInstance().GetCurrentTimeInFormat()),
						fmt::arg("level", logLevelNames[static_cast<unsigned short> (logLevel)]), fmt::arg("file", file),
						fmt::arg("function", function), fmt::arg("line", line), fmt::arg("log", message));

				logQueue.Push(formattedLog);

				if (logQueue.Size() > flushSize) {
					Flush();
				}
			}

			void Flush() {
				if (loggingtoStdOutEnabled) {
					FlushToStdOut();
				}
				if (loggingToFileEnabled) {
					FlushToFile();
				}
			}

			std::string GetLogFormat() const {
				return logFormat;
			}

			void SetLogFormat(std::string logFormat) {
				this->logFormat = logFormat;
			}

			bool IsLoggingToFileEnabled() const {
				return loggingToFileEnabled;
			}

			void SetLoggingToFileEnabled(bool loggingToFileEnabled) {
				this->loggingToFileEnabled = loggingToFileEnabled;
			}

			bool IsLoggingtoStdOutEnabled() const {
				return loggingtoStdOutEnabled;
			}

			void SetLoggingtoStdOutEnabled(bool loggingtoStdOutEnabled) {
				this->loggingtoStdOutEnabled = loggingtoStdOutEnabled;
			}
		};

		const std::array<std::string, 6> NewLogger::logLevelNames = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "CRITICAL ERROR"};
	}
}

#endif /* NEWLOGGER_HPP */

