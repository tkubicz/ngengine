/* 
 * File:   LogConfig.hpp
 * Author: tku
 *
 * Created on 24 March 2016, 15:13
 */

#ifndef LOGCONFIG_HPP
#define LOGCONFIG_HPP

#include "NGE/Tools/Logger/LogTypes.hpp"

namespace NGE {
	namespace Tools {
		namespace Logger {

			struct LogConfig {
				/**
				 * Format of the logged message
				 */
				std::string logFormat;

				/**
				 * Format of the date used in logger.
				 */
				std::string dateFormat;

				/**
				 * Flag that indicates if the automatic flushing is enabled.
				 */
				bool autoFlushEnabled;

				/**
				 * Default maximum size of the log queue.
				 */
				unsigned int flushAfter;

				/**
				 * Current logging level.
				 */
				LogTypes::LOG_LEVEL logLevel;

				LogConfig() { }

				LogConfig(std::string logFormat, std::string dateFormat, bool autoFlushEnabled, unsigned int flushAfter, LogTypes::LOG_LEVEL logLevel) :
				logFormat(logFormat), dateFormat(dateFormat), autoFlushEnabled(autoFlushEnabled), flushAfter(flushAfter), logLevel(logLevel) { }

			};
		}
	}
}

#endif /* LOGCONFIG_HPP */

