/* 
 * File:   LoggerOutput.hpp
 * Author: tku
 *
 * Created on 21 March 2016, 02:01
 */

#ifndef LOGGEROUTPUT_HPP
#define LOGGEROUTPUT_HPP

#include <mutex>
#include "NGE/Tools/Logger/LogLevel.hpp"
#include "NGE/Tools/Logger/LogMessage.hpp"

namespace NGE {
	namespace Tools {
		namespace Logger {

			class LoggerOutput {
			  protected:
				/**
				 * Current log level.
				 */
				LogLevel::LOG_LEVEL logLevel;

				/**
				 * Queue that keeps log messages. It is cleared after flush.
				 */
				NGE::Core::ConcurrentQueue<LogMessage> queue;

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

				LoggerOutput(LogLevel::LOG_LEVEL logLevel, std::string logFormat, std::string dateFormat, unsigned int flushAfter, bool enabled) :
				logLevel(logLevel), logFormat(logFormat), dateFormat(dateFormat), flushAfter(flushAfter), enabled(enabled) { }

				virtual ~LoggerOutput() {
					if (queue.Empty()) {
						queue.Clear();
					}
				}

				virtual void Init() { }

				virtual void Flush() { }

				NGE::Core::ConcurrentQueue<LogMessage>& GetQueue() {
					return queue;
				}

				bool IsEnabled() const {
					return enabled;
				}

				void SetEnabled(bool enabled) {
					this->enabled = enabled;
				}
			};
		}
	}
}

#endif /* LOGGEROUTPUT_HPP */

