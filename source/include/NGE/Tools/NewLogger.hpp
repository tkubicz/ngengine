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
#include <cppformat/format.h>
#include "NGE/Core/ConcurrentQueue.hpp"
#include "NGE/Tools/Timing.hpp"

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
			 * Holds currently accumulated logs. This structure is cleared
			 * after flushing.
			 */
			NGE::Core::ConcurrentQueue<std::string> logStrings;

			/**
			 * Format of the logged message
			 */
			std::string logFormat;

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

		  public:

			static NewLogger& GetInstance() {
				static NewLogger instance;
				return instance;
			}

			void Initialise() {
				logLevel = LOG_LEVEL::TRACE;
				loggingToFileEnabled = false;
				loggingtoStdOutEnabled = true;
				logFormat = "{} - {} - {} - {}[{}]: ";

				if (!logStrings.Empty()) {
					logStrings.Clear();
				}
			}

			template<typename... Args> void WriteLog(const std::string& format, LOG_LEVEL logLevel, Args&&... args) {
				//logStrings.Push(fmt::format())
			}

			template<typename... Args> void WriteLog(const std::string& format, LOG_LEVEL logLevel, const char* file, int line, const char* function, Args&&... arguments) {

				if (logStrings.Size() > 20) {
					FlushToFile();
				}

				logStrings.Push(fmt::format(logFormat + format, Timing::GetInstance().GetCurrentTimeInFormat(),
						logLevelNames[static_cast<unsigned short> (logLevel)], file, function, line, std::forward<Args>(arguments)...));
			}

			void FlushToStdOut() {
				while (logStrings.Size() > 0) {
					std::string logMessage;
					if (logStrings.TryPop(logMessage)) {
						std::cout << logMessage << std::endl;
					}
				}
			}

			void FlushToFile() {
				std::ofstream logFile("log.txt", std::ios::app);
				while (logStrings.Size() > 0) {
					std::string logMessage;
					if (logStrings.TryPop(logMessage)) {
						logFile << logMessage << "\n";
					}
				}
			}

			std::string GetLogFormat() const {
				return logFormat;
			}

			void SetLogFormat(std::string logFormat) {
				this->logFormat = logFormat;
			}
		};

		const std::array<std::string, 6> NewLogger::logLevelNames = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "CRITICAL ERROR"};
	}
}

#define test_trace_log(format, ...) NGE::Tools::NewLogger::GetInstance().WriteLog(format, NGE::Tools::NewLogger::LOG_LEVEL::TRACE, __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__);
#define test_debug_log(format, ...) NGE::Tools::NewLogger::GetInstance().WriteLog(format, NGE::Tools::NewLogger::LOG_LEVEL::DEBUG, __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__);
#define test_info_log(format, ...) NGE::Tools::NewLogger::GetInstance().WriteLog(format, NGE::Tools::NewLogger::LOG_LEVEL::INFO, __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__);
#define test_warn_log(format, ...) NGE::Tools::NewLogger::GetInstance().WriteLog(format, NGE::Tools::NewLogger::LOG_LEVEL::WARN, __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__);
#define test_error_log(format, ...) NGE::Tools::NewLogger::GetInstance().WriteLog(format, NGE::Tools::NewLogger::LOG_LEVEL::ERROR, __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__);
#define test_critical_log(format, ...) NGE::Tools::NewLogger::GetInstance().WriteLog(format, NGE::Tools::NewLogger::LOG_LEVEL::CRITICAL_ERROR, __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__);



#endif /* NEWLOGGER_HPP */

