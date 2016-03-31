/* 
 * File:   LogMessage.hpp
 * Author: tku
 *
 * Created on 22 marca 2016, 15:16
 */

#ifndef LOGMESSAGE_HPP
#define LOGMESSAGE_HPP

#include <chrono>
#include "NGE/Tools/Logger/LogTypes.hpp"
#include "NGE/Tools/Timing.hpp"

namespace NGE {
	namespace Tools {
		namespace Logger {

			class LogMessage {
			  public:
				NGE::Tools::Logger::LogTypes::LOG_LEVEL logLevel;
				std::chrono::milliseconds timeInMs;
				std::string file;
				std::string function;
				unsigned int line;
				std::string message;

			  public:

				LogMessage() { }

				LogMessage(NGE::Tools::Logger::LogTypes::LOG_LEVEL logLevel, const std::string& file, const std::string& function, unsigned int line, const std::string& message) :
				logLevel(logLevel), file(file), function(function), line(line), message(message) {
					timeInMs = Timing::GetInstance().GetCurrentTimeInMs();
				}
			};
		}
	}
}

#endif /* LOGMESSAGE_HPP */

