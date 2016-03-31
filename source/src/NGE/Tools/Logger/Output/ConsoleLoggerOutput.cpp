#include "NGE/Tools/Logger/Output/ConsoleLoggerOutput.hpp"
using namespace NGE::Tools::Logger::Output;

ConsoleLoggerOutput::ConsoleLoggerOutput(LogTypes::LOG_LEVEL logLevel, std::string logFormat, std::string dateFormat, bool autoFlushEnabled, unsigned int flushAfter, bool enabled) :
AbstractLoggerOutput(logLevel, logFormat, dateFormat, autoFlushEnabled, flushAfter, enabled) {
}

ConsoleLoggerOutput::ConsoleLoggerOutput(const LogConfig& logConfig) :
AbstractLoggerOutput(logConfig) {
}

void ConsoleLoggerOutput::Flush() {
	std::unique_lock<std::mutex> lock(mutex);
	fmt::MemoryWriter mw;
	BuildMemoryWriterFromQueue(mw);
	std::cout << mw.c_str();
	mw.clear();
}


