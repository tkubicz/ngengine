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
					ConsoleLoggerOutput(LogTypes::LOG_LEVEL logLevel, std::string logFormat, std::string dateFormat, bool autoFlushEnabled = true, unsigned int flushAfter = 20, bool enabled = true);
					ConsoleLoggerOutput(const LogConfig& logConfig);
					virtual void Flush() override;
				};
			}
		}
	}
}

#endif /* CONSOLELOGGEROUTPUT_HPP */

