/* 
 * File:   Logger.hpp
 * Author: tku
 *
 * Created on 15 styczeń 2012, 23:24
 */

#ifndef LOGGER_HPP
#define	LOGGER_HPP

#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>

#define nge_to_string(x) boost::lexical_cast<std::string>(x)
#define nge_log_info(x) NGE::Tools::Logger::WriteInfoLog(x)
#define nge_log_error(x) NGE::Tools::Logger::WriteErrorLog(x)

#ifdef ANDROID
#include <android/log.h>
#define  LOG_TAG    "no-gravity-engine"
#define	 LOGI(...)  __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define	 LOGW(...)  __android_log_print(ANDROID_LOG_WARN,  LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif

namespace NGE {
    namespace Tools {

        class Logger {
          protected:
            // TODO: Change it to list. Vector is not needed here.
            static std::vector<std::string> logStrings;
            static std::string logPath;
            static bool showOutput;

            static void ShowOutput();

          public:
            static void Initialize(const std::string &filename = "", bool output = false);
            static void Flush();

            static void WriteImmidiatleInfoLog(const std::string &info);
            static void WriteFatalErrorLog(const std::string &error);
            static void WriteErrorLog(const std::string &error);
            static void WriteInfoLog(const std::string &info);
        };
    }
}

#endif	/* LOGGER_HPP */

