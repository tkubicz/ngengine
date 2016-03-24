/* 
 * File:   StreamLoggerOutput.hpp
 * Author: tku
 *
 * Created on 24 March 2016, 13:18
 */

#ifndef STREAMLOGGEROUTPUT_HPP
#define STREAMLOGGEROUTPUT_HPP

#include <ostream>
#include "NGE/Tools/Logger/Output/AbstractLoggerOutput.hpp"

namespace NGE {
	namespace Tools {
		namespace Logger {
			namespace Output {

				class StreamLoggerOutput : public AbstractLoggerOutput {
				  private:

					std::ostream* outputStream;

				  public:

					StreamLoggerOutput(LogTypes::LOG_LEVEL logLevel, std::string logFormat, std::string dateFormat, unsigned int flushAfter, bool enabled) :
					AbstractLoggerOutput(logLevel, logFormat, dateFormat, flushAfter, enabled) { }

					virtual void Flush() override {
						
					}

					std::ostream* GetOutputStream() const {
						return outputStream;
					}

					void SetOutputStream(std::ostream* outputStream) {
						this->outputStream = outputStream;
					}
				};
			}
		}
	}
}

#endif /* STREAMLOGGEROUTPUT_HPP */

