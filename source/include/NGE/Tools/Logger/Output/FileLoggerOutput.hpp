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
#include "NGE/Tools/Logger/Output/LoggerOutput.hpp"

namespace NGE {
	namespace Tools {
		namespace Logger {
			namespace Output {

				class FileLoggerOutput : public LoggerOutput {
				  protected:
					std::ofstream file;

					std::string filePath;

				  public:

					FileLoggerOutput(LogTypes::LOG_LEVEL logLevel, std::string logFormat, std::string dateFormat, unsigned int flushAfter, bool enabled) :
					LoggerOutput(logLevel, logFormat, dateFormat, flushAfter, enabled) {
						filePath = "nge.log";
					}

					virtual void Flush() override {
						std::unique_lock<std::mutex> lock(mutex);

						file.open(filePath, std::ios::app);
						if (!file.is_open()) {
							//log_error("Could not open log file: '{}'", filePath);
							std::cout << "Cold not open log file: '" << filePath << "'\n";
							return;
						}

						fmt::MemoryWriter mw;
						queue.DrainTo(internalQueue);
						while (!internalQueue.empty()) {
							LogMessage logMsg = internalQueue.front();
							internalQueue.pop();
							std::string formattedLog = FormatLogMessage(logMsg);
							mw << formattedLog << "\n";
						}

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

