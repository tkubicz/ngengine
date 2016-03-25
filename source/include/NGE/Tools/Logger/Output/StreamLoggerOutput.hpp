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
					StreamLoggerOutput(LogTypes::LOG_LEVEL logLevel, std::string logFormat, std::string dateFormat, bool autoFlushEnabled = true, unsigned int flushAfter = 20, bool enabled = true);
					StreamLoggerOutput(const LogConfig& logConfig);
					virtual void Flush() override;

					std::ostream* GetOutputStream();
					void SetOutputStream(std::ostream* outputStream);
				};
			}
		}
	}
}

#endif /* STREAMLOGGEROUTPUT_HPP */

