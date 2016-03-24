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
#include "NGE/Tools/Logger/Output/AbstractLoggerOutput.hpp"

namespace NGE {
	namespace Tools {
		namespace Logger {
			namespace Output {

				class ConsoleLoggerOutput : public AbstractLoggerOutput {
				  public:

					ConsoleLoggerOutput(LogTypes::LOG_LEVEL logLevel, std::string logFormat, std::string dateFormat, bool autoFlushEnabled, unsigned int flushAfter, bool enabled) :
					AbstractLoggerOutput(logLevel, logFormat, dateFormat, autoFlushEnabled, flushAfter, enabled) { }

					ConsoleLoggerOutput(LogConfig logConfig) :
					AbstractLoggerOutput(logConfig) { }

					virtual void Flush() override {
						std::unique_lock<std::mutex> lock(mutex);
						fmt::MemoryWriter mw;
						BuildMemoryWriterFromQueue(mw);
						std::cout << mw.c_str();
						mw.clear();
					}
				};
			}
		}
	}
}

#endif /* CONSOLELOGGEROUTPUT_HPP */

