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

				  protected:

					void SetDefaultFilePath() {
						filePath = "nge.log";
					}

				  public:

					FileLoggerOutput(LogTypes::LOG_LEVEL logLevel, std::string logFormat, std::string dateFormat, bool autoFlushEnabled, unsigned int flushAfter, bool enabled) :
					AbstractLoggerOutput(logLevel, logFormat, dateFormat, autoFlushEnabled, flushAfter, enabled) {
						SetDefaultFilePath();
					}

					FileLoggerOutput(LogConfig logConfig) :
					AbstractLoggerOutput(logConfig) {
						SetDefaultFilePath();
					}

					virtual void Flush() override {
						std::unique_lock<std::mutex> lock(mutex);

						file.open(filePath, std::ios::app);
						if (!file.is_open()) {
							log_error("Could not open log file: '{}'", filePath);
							return;
						}

						fmt::MemoryWriter mw;
						BuildMemoryWriterFromQueue(mw);

						file << mw.c_str();
						file.close();
						mw.clear();
					}

					std::string GetFilePath() const {
						return filePath;
					}

					void SetFilePath(const std::string& filePath) {
						this->filePath = filePath;
					}
				};
			}
		}
	}
}

#endif /* FILELOGGEROUTPUT_HPP */

