/* 
 * File:   AndroidLoggerOutput.hpp
 * Author: tku
 *
 * Created on 23 March 2016, 02:11
 */

#ifndef ANDROIDLOGGEROUTPUT_HPP
#define ANDROIDLOGGEROUTPUT_HPP

#include "LoggerOutput.hpp"

// TODO: Fix and finish that.
// This is just temporary here.
#ifdef ANDROID
#include <android/log.h>
#define  LOG_TAG    "no-gravity-engine"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,  LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif

namespace NGE {
    namespace Tools {
        namespace Logger {

            class AndroidLoggerOutput : public LoggerOutput {
              public:

                AndroidLoggerOutput(LogTypes::LOG_LEVEL logLevel, std::string logFormat, std::string dateFormat, unsigned int flushAfter, bool enabled) :
                LoggerOutput(logLevel, logFormat, dateFormat, flushAfter, enabled) { }

                virtual void Init() { }

                virtual void Flush() { }
            };
        }
    }
}

#endif /* ANDROIDLOGGEROUTPUT_HPP */

