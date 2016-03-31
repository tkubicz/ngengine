#include "NGE/Tools/Logger/Output/StreamLoggerOutput.hpp"
using namespace NGE::Tools::Logger::Output;

StreamLoggerOutput::StreamLoggerOutput(LogTypes::LOG_LEVEL logLevel, std::string logFormat, std::string dateFormat, bool autoFlushEnabled, unsigned int flushAfter, bool enabled) :
AbstractLoggerOutput(logLevel, logFormat, dateFormat, autoFlushEnabled, flushAfter, enabled) {
}

StreamLoggerOutput::StreamLoggerOutput(const LogConfig& logConfig) :
AbstractLoggerOutput(logConfig) {
}

void StreamLoggerOutput::Flush() {
	std::unique_lock<std::mutex> lock(mutex);
	fmt::MemoryWriter mw;
	BuildMemoryWriterFromQueue(mw);
	outputStream->write(mw.c_str(), mw.size());
	mw.clear();
}

std::ostream* StreamLoggerOutput::GetOutputStream() {
	return outputStream;
}

void StreamLoggerOutput::SetOutputStream(std::ostream* outputStream) {
	this->outputStream = outputStream;
}




