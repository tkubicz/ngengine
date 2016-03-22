/* 
 * File:   ConsoleLoggerOutput.hpp
 * Author: tku
 *
 * Created on 22 March 2016, 11:32
 */

#ifndef CONSOLELOGGEROUTPUT_HPP
#define CONSOLELOGGEROUTPUT_HPP

#include <iostream>
#include <queue>
#include <cppformat/format.h>
#include "NGE/Tools/Logger/LoggerOutput.hpp"

namespace NGE {
	namespace Tools {
		namespace Logger {

			class ConsoleLoggerOutput : public LoggerOutput {
			  public:

				ConsoleLoggerOutput(LogLevel::LOG_LEVEL logLevel, std::string logFormat, std::string dateFormat, unsigned int flushAfter, bool enabled) :
				LoggerOutput(logLevel, logFormat, dateFormat, flushAfter, enabled) { }

				virtual void Init() override { }

				virtual void Flush() override {
					std::unique_lock<std::mutex> lock(mutex);
					while (!queue.Empty()) {
						LogMessage logMsg;
						queue.TryPop(logMsg);

						std::string formattedLog = fmt::format(logFormat,
								fmt::arg("date", Timing::GetInstance().GetTimeInFormat(logMsg.timeInMs, dateFormat)),
								fmt::arg("level", LogLevel::LOG_LEVEL_NAME[static_cast<unsigned short> (logMsg.logLevel)]),
								fmt::arg("file", logMsg.file),
								fmt::arg("function", logMsg.function),
								fmt::arg("line", logMsg.line),
								fmt::arg("log", logMsg.message));

						std::cout << formattedLog << "\n";
					}
				}
			};
		}
	}
}

#endif /* CONSOLELOGGEROUTPUT_HPP */

