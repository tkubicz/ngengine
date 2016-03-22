/* 
 * File:   LogLevel.hpp
 * Author: tku
 *
 * Created on 22 marca 2016, 14:58
 */

#ifndef LOGLEVEL_HPP
#define LOGLEVEL_HPP

#include <array>

namespace NGE {
	namespace Tools {
		namespace Logger {

			struct LogLevel {

				enum class LOG_LEVEL : unsigned short {
					TRACE = 0, DEBUG = 1, INFO = 2, WARN = 3, ERROR = 4, CRITICAL_ERROR = 5
				};

				static const std::array<std::string, 6> LOG_LEVEL_NAME;
			};

			const std::array<std::string, 6> LogLevel::LOG_LEVEL_NAME = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "CRITICAL ERROR"};
		}
	}
}

#endif /* LOGLEVEL_HPP */

