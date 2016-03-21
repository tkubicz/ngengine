/* 
 * File:   LoggerOutput.hpp
 * Author: tku
 *
 * Created on 21 marca 2016, 02:01
 */

#ifndef LOGGEROUTPUT_HPP
#define LOGGEROUTPUT_HPP

#include "NGE/Tools/NewLogger.hpp"

namespace NGE {
    namespace Tools {
        namespace Logger {

            class LoggerOutput {
              protected:
                NewLogger::LOG_LEVEL level;

                NGE::Core::ConcurrentQueue<std::string> queue;

                std::string logFormat;

                std::string dateFormat;

                unsigned int maxQueueSize;

                std::shared_ptr<NewLogger> globalLogger;

              public:

                virtual void Flush() = 0;
            };
        }
    }
}

#endif /* LOGGEROUTPUT_HPP */

