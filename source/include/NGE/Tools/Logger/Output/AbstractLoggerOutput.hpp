/* 
 * File:   LoggerOutput.hpp
 * Author: tku
 *
 * Created on 21 March 2016, 02:01
 */

#ifndef LOGGEROUTPUT_HPP
#define LOGGEROUTPUT_HPP

#include <mutex>
#include "cppformat/format.h"
#include "NGE/Core/ConcurrentQueue.hpp"
#include "NGE/Parsers/ParsingUtils.hpp"
#include "NGE/Tools/Logger/LogConfig.hpp"
#include "NGE/Tools/Logger/LogTypes.hpp"
#include "NGE/Tools/Logger/LogMessage.hpp"

namespace NGE {
	namespace Tools {
		namespace Logger {
			namespace Output {

				class AbstractLoggerOutput {
				  protected:
					/**
					 * Queue that keeps log messages. It is cleared after flush.
					 */
					NGE::Core::ConcurrentQueue<std::shared_ptr<LogMessage>> queue;

					/**
					 * Internal queue.
					 */
					std::queue<std::shared_ptr<LogMessage>> internalQueue;

					/**
					 * Mutex used to lock the flushing.
					 */
					std::mutex mutex;

					/**
					 * Is the Logger enabled.
					 */
					bool enabled;

					LogConfig logConfig;

				  public:

					AbstractLoggerOutput(LogTypes::LOG_LEVEL logLevel, std::string logFormat, std::string dateFormat, bool autoFlushEnabled, unsigned int flushAfter, bool enabled);

					AbstractLoggerOutput(LogConfig logConfig);

					virtual ~AbstractLoggerOutput();

					virtual void Flush() = 0;

					NGE::Core::ConcurrentQueue<std::shared_ptr<LogMessage>>&GetQueue();

					bool IsEnabled() const;

					void SetEnabled(bool enabled);

					LogTypes::LOG_LEVEL GetLogLevel() const;

					void SetLogLevel(LogTypes::LOG_LEVEL logLevel);

					std::string GetLogFormat() const;

					void SetLogFormat(std::string logFormat);

					std::string GetDateFormat() const;

					void SetDateFormat(std::string dateFormat);

					LogConfig GetLogConfig() const;

					void SetLogConfig(LogConfig logConfig);

					bool IsAutoFlushEnabled() const;

					void SetAutoFlushEnabled(bool autoFlushEnabled);

					unsigned int GetFlushAfter() const;

					void SetFlushAfter(unsigned int flushAfter);

				  protected:

					std::string FormatLogMessage(const LogMessage& logMsg);

					std::string GetShortFileName(const std::string& fileName);

					std::string GetMethodWithArguments(const std::string& function);

					std::string GetMethodWithoutArguments(const std::string& function);

					void BuildMemoryWriterFromQueue(fmt::MemoryWriter& mw);
				};
			}
		}
	}
}

#endif /* LOGGEROUTPUT_HPP */

