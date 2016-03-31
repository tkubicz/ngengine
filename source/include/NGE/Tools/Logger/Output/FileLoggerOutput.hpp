/* 
 * File:   FileLoggerOutput.hpp
 * Author: tku
 *
 * Created on 21 March 2016, 02:06
 */

#ifndef FILELOGGEROUTPUT_HPP
#define FILELOGGEROUTPUT_HPP

#include <iostream>
#include <fstream>
#include "NGE/Tools/Logger/NewLogger.hpp"
#include "NGE/Tools/Logger/Output/AbstractLoggerOutput.hpp"

namespace NGE {
	namespace Tools {
		namespace Logger {
			namespace Output {

				class FileLoggerOutput : public AbstractLoggerOutput {
				  protected:
					std::ofstream file;

					std::string filePath;

					static const std::string defaultFilePath;

				  protected:
					void SetDefaultFilePath();

				  public:
					FileLoggerOutput(LogTypes::LOG_LEVEL logLevel, std::string logFormat, std::string dateFormat, bool autoFlushEnabled = true, unsigned int flushAfter = 20, bool enabled = true);
					FileLoggerOutput(const LogConfig& logConfig);

					virtual void Flush() override;

					std::string GetFilePath() const;
					void SetFilePath(const std::string& filePath);
				};
			}
		}
	}
}

#endif /* FILELOGGEROUTPUT_HPP */

