/* 
 * File:   ConsoleLoggerOutput.hpp
 * Author: tku
 *
 * Created on 22 March 2016, 11:32
 */

#ifndef CONSOLELOGGEROUTPUT_HPP
#define CONSOLELOGGEROUTPUT_HPP

#include <iostream>
#include <queue>
#include <cppformat/format.h>
#include "NGE/Tools/Logger/LoggerOutput.hpp"

namespace NGE {
    namespace Tools {
        namespace Logger {

            class ConsoleLoggerOutput : public LoggerOutput {
              public:

                ConsoleLoggerOutput(LogTypes::LOG_LEVEL logLevel, std::string logFormat, std::string dateFormat, unsigned int flushAfter, bool enabled) :
                LoggerOutput(logLevel, logFormat, dateFormat, flushAfter, enabled) { }

                virtual void Init() override { }

                virtual void Flush() override {
                    std::unique_lock<std::mutex> lock(mutex);

                    fmt::MemoryWriter mw;
                    queue.DrainTo(internalQueue);
                    while (!internalQueue.empty()) {
                        LogMessage logMsg = internalQueue.front();
                        internalQueue.pop();
                        std::string formattedLog = FormatLogMessage(logMsg);
                        mw << formattedLog << "\n";
                    }

                    std::cout << mw.c_str();
                    mw.clear();
                }
            };
        }
    }
}

#endif /* CONSOLELOGGEROUTPUT_HPP */

