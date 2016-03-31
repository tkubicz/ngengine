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
				 * Current logging level.
				 */
				LogTypes::LOG_LEVEL logLevel;

				/**
				 * Flag that indicates if the logger is enabled.
				 */
				bool enabled;

				/**
				 * Flag that indicates if the automatic flushing is enabled.
				 */
				bool autoFlushEnabled;

				/**
				 * Default maximum size of the log queue.
				 */
				unsigned int flushAfter;

				LogConfig() { }

				LogConfig(std::string logFormat, std::string dateFormat, LogTypes::LOG_LEVEL logLevel, bool enabled, bool autoFlushEnabled, unsigned int flushAfter) :
				logFormat(logFormat), dateFormat(dateFormat), logLevel(logLevel), enabled(enabled), autoFlushEnabled(autoFlushEnabled), flushAfter(flushAfter) { }

			};
		}
	}
}

#endif /* LOGCONFIG_HPP */

