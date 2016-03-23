/* 
 * File:   LogLevel.hpp
 * Author: tku
 *
 * Created on 22 March 2016, 14:58
 */

#ifndef LOGTYPES_HPP
#define LOGTYPES_HPP

#include <array>

namespace NGE {
	namespace Tools {
		namespace Logger {

			struct LogTypes {

				enum class LOG_LEVEL : unsigned short {
					TRACE = 0, DEBUG = 1, INFO = 2, WARN = 3, ERROR = 4, CRITICAL = 5
				};

				static const std::array<std::string, 6> LOG_LEVEL_NAME;
			};
		}
	}
}

#endif /* LOGTYPES_HPP */

