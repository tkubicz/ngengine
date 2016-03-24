/* 
 * File:   LoggerOutput.hpp
 * Author: tku
 *
 * Created on 21 March 2016, 02:01
 */

#ifndef LOGGEROUTPUT_HPP
#define LOGGEROUTPUT_HPP

#include <mutex>
#include "NGE/Parsers/ParsingUtils.hpp"
#include "NGE/Tools/Logger/LogTypes.hpp"
#include "NGE/Tools/Logger/LogMessage.hpp"

namespace NGE {
	namespace Tools {
		namespace Logger {
			namespace Output {

				class AbstractLoggerOutput {
				  protected:
					/**
					 * Current log level.
					 */
					LogTypes::LOG_LEVEL logLevel;

					/**
					 * Queue that keeps log messages. It is cleared after flush.
					 */
					NGE::Core::ConcurrentQueue<std::shared_ptr<LogMessage>> queue;

					/**
					 * Internal queue.
					 */
					std::queue<std::shared_ptr<LogMessage>> internalQueue;

					/**
					 * Format of the log.
					 */
					std::string logFormat;

					/**
					 * Format of the date.
					 */
					std::string dateFormat;

					/**
					 * Maximum size of the log queue. When queue size reaches
					 * size defined here, it will be flushed.
					 */
					unsigned int flushAfter;

					/**
					 * Mutex used to lock the flushing.
					 */
					std::mutex mutex;

					/**
					 * Is the Logger enabled.
					 */
					bool enabled;

				  public:

					AbstractLoggerOutput(LogTypes::LOG_LEVEL logLevel, std::string logFormat, std::string dateFormat, unsigned int flushAfter, bool enabled) :
					logLevel(logLevel), logFormat(logFormat), dateFormat(dateFormat), flushAfter(flushAfter), enabled(enabled) { }

					virtual ~AbstractLoggerOutput() {
						if (queue.Empty()) {
							queue.Clear();
						}
					}

					virtual void Flush() = 0;

					NGE::Core::ConcurrentQueue<std::shared_ptr<LogMessage>>&GetQueue() {
						return queue;
					}

					bool IsEnabled() const {
						return enabled;
					}

					void SetEnabled(bool enabled) {
						this->enabled = enabled;
					}

					LogTypes::LOG_LEVEL GetLogLevel() const {
						return logLevel;
					}

					void SetLogLevel(LogTypes::LOG_LEVEL logLevel) {
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


				  protected:

					std::string FormatLogMessage(const LogMessage& logMsg) {
						return fmt::format(logFormat,
								fmt::arg("date", Timing::GetInstance().GetTimeInFormat(logMsg.timeInMs, dateFormat)),
								fmt::arg("level", LogTypes::LOG_LEVEL_NAME[static_cast<unsigned short> (logMsg.logLevel)]),
								fmt::arg("file", logMsg.file),
								fmt::arg("shortFile", GetShortFileName(logMsg.file)),
								fmt::arg("function", logMsg.function),
								fmt::arg("shortFunction", GetMethodWithoutArguments(logMsg.function)),
								fmt::arg("line", logMsg.line),
								fmt::arg("log", logMsg.message));
					}

					std::string GetShortFileName(const std::string& fileName) {
						return fileName.substr(fileName.find_last_of(NGE::Parsers::ParsingUtils::PATH_SEPARATOR) + 1);
					}

					std::string GetMethodWithArguments(const std::string& function) {
						return function.substr(function.find_last_of("::", function.find_first_of('(')) + 1);
					}

					std::string GetMethodWithoutArguments(const std::string& function) {
						std::string methodWithArguments = GetMethodWithArguments(function);
						return methodWithArguments.substr(0, methodWithArguments.find_first_of('('));
					}
				};
			}
		}
	}
}

#endif /* LOGGEROUTPUT_HPP */

