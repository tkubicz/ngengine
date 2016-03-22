/* 
 * File:   FileLoggerOutput.hpp
 * Author: tku
 *
 * Created on 21 March 2016, 02:06
 */

#ifndef FILELOGGEROUTPUT_HPP
#define FILELOGGEROUTPUT_HPP

#include <fstream>
#include "NGE/Tools/Logger/LoggerOutput.hpp"

namespace NGE {
	namespace Tools {
		namespace Logger {

			class FileLoggerOutput : public LoggerOutput {
			  protected:
				std::ofstream file;

			  public:

				FileLoggerOutput(LogLevel::LOG_LEVEL logLevel, std::string logFormat, std::string dateFormat, unsigned int flushAfter, bool enabled) :
				LoggerOutput(logLevel, logFormat, dateFormat, flushAfter, enabled) {
				}

				virtual void Init() override { }

				virtual void Flush() override { }
			};
		}
	}
}

#endif /* FILELOGGEROUTPUT_HPP */

