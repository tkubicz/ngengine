/* 
 * File:   FileLoggerOutput.hpp
 * Author: tku
 *
 * Created on 21 marca 2016, 02:06
 */

#ifndef FILELOGGEROUTPUT_HPP
#define FILELOGGEROUTPUT_HPP

#include "LoggerOutput.hpp"


namespace NGE {
    namespace Tools {
        namespace Logger {

            class FileLoggerOutput : public LoggerOutput {
              public:

                void Flush() override { }
            };
        }
    }
}

#endif /* FILELOGGEROUTPUT_HPP */

