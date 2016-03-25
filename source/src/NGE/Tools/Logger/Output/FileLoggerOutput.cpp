#include "NGE/Tools/Logger/Output/FileLoggerOutput.hpp"
using namespace NGE::Tools::Logger::Output;

const std::string FileLoggerOutput::defaultFilePath = "nge.log";

FileLoggerOutput::FileLoggerOutput(LogTypes::LOG_LEVEL logLevel, std::string logFormat, std::string dateFormat, bool autoFlushEnabled, unsigned int flushAfter, bool enabled) :
AbstractLoggerOutput(logLevel, logFormat, dateFormat, autoFlushEnabled, flushAfter, enabled) {
	SetDefaultFilePath();
}

FileLoggerOutput::FileLoggerOutput(const LogConfig& logConfig) :
AbstractLoggerOutput(logConfig) {
	SetDefaultFilePath();
}

void FileLoggerOutput::SetDefaultFilePath() {
	filePath = defaultFilePath;
}

void FileLoggerOutput::Flush() {
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

std::string FileLoggerOutput::GetFilePath() const {
	return filePath;
}

void FileLoggerOutput::SetFilePath(const std::string& filePath) {
	this->filePath = filePath;
}


