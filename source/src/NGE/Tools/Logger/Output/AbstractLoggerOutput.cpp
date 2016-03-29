#include "NGE/Tools/Logger/Output/AbstractLoggerOutput.hpp"
#include "cppformat/format.h"
using namespace NGE::Tools::Logger::Output;

namespace l = NGE::Tools::Logger;

AbstractLoggerOutput::AbstractLoggerOutput(LogTypes::LOG_LEVEL logLevel, std::string logFormat, std::string dateFormat, bool autoFlushEnabled, unsigned int flushAfter, bool enabled) {
	logConfig.logLevel = logLevel;
	logConfig.logFormat = logFormat;
	logConfig.dateFormat = dateFormat;
	logConfig.autoFlushEnabled = autoFlushEnabled;
	logConfig.flushAfter = flushAfter;
	logConfig.enabled = enabled;
}

AbstractLoggerOutput::AbstractLoggerOutput(const LogConfig& logConfig) :
logConfig(logConfig) {
}

AbstractLoggerOutput::~AbstractLoggerOutput() {
	if (queue.Empty()) {
		queue.Clear();
	}
}

void AbstractLoggerOutput::TryFlush() {
	if (FlushNow()) {
		Flush();
	}
}

NGE::Core::ConcurrentQueue<std::shared_ptr<l::LogMessage>>&AbstractLoggerOutput::GetQueue() {
	return queue;
}

bool AbstractLoggerOutput::IsEnabled() const {
	return logConfig.enabled;
}

void AbstractLoggerOutput::SetEnabled(bool enabled) {
	this->logConfig.enabled = enabled;
}

l::LogTypes::LOG_LEVEL AbstractLoggerOutput::GetLogLevel() const {
	return logConfig.logLevel;
}

void AbstractLoggerOutput::SetLogLevel(l::LogTypes::LOG_LEVEL logLevel) {
	this->logConfig.logLevel = logLevel;
}

std::string AbstractLoggerOutput::GetLogFormat() const {
	return logConfig.logFormat;
}

void AbstractLoggerOutput::SetLogFormat(std::string logFormat) {
	this->logConfig.logFormat = logFormat;
}

std::string AbstractLoggerOutput::GetDateFormat() const {
	return logConfig.dateFormat;
}

void AbstractLoggerOutput::SetDateFormat(std::string dateFormat) {
	this->logConfig.dateFormat = dateFormat;
}

l::LogConfig AbstractLoggerOutput::GetLogConfig() const {
	return logConfig;
}

void AbstractLoggerOutput::SetLogConfig(LogConfig logConfig) {
	this->logConfig = logConfig;
}

bool AbstractLoggerOutput::IsAutoFlushEnabled() const {
	return this->logConfig.autoFlushEnabled;
}

void AbstractLoggerOutput::SetAutoFlushEnabled(bool autoFlushEnabled) {
	this->logConfig.autoFlushEnabled = autoFlushEnabled;
}

unsigned int AbstractLoggerOutput::GetFlushAfter() const {
	return this->logConfig.flushAfter;
}

void AbstractLoggerOutput::SetFlushAfter(unsigned int flushAfter) {
	this->logConfig.flushAfter = flushAfter;
}

std::string AbstractLoggerOutput::FormatLogMessage(const LogMessage& logMsg) {
	return fmt::format(logConfig.logFormat,
			fmt::arg("date", Timing::GetInstance().GetTimeInFormat(logMsg.timeInMs, logConfig.dateFormat)),
			fmt::arg("level", LogTypes::LOG_LEVEL_NAME[static_cast<unsigned short> (logMsg.logLevel)]),
			fmt::arg("file", logMsg.file),
			fmt::arg("shortFile", GetShortFileName(logMsg.file)),
			fmt::arg("function", logMsg.function),
			fmt::arg("shortFunction", GetMethodWithoutArguments(logMsg.function)),
			fmt::arg("line", logMsg.line),
			fmt::arg("log", logMsg.message));
}

std::string AbstractLoggerOutput::GetShortFileName(const std::string& fileName) {
	return fileName.substr(fileName.find_last_of(NGE::Parsers::ParsingUtils::PATH_SEPARATOR) + 1);
}

std::string AbstractLoggerOutput::GetMethodWithArguments(const std::string& function) {
	return function.substr(function.find_last_of("::", function.find_first_of('(')) + 1);
}

std::string AbstractLoggerOutput::GetMethodWithoutArguments(const std::string& function) {
	std::string methodWithArguments = GetMethodWithArguments(function);
	return methodWithArguments.substr(0, methodWithArguments.find_first_of('('));
}

void AbstractLoggerOutput::BuildMemoryWriterFromQueue(fmt::MemoryWriter& mw) {
	queue.DrainTo(internalQueue);
	mw.clear();
	while (!internalQueue.empty()) {
		std::shared_ptr<LogMessage> logMsg = internalQueue.front();
		internalQueue.pop();
		std::string formattedLog = FormatLogMessage(*logMsg.get());
		mw << formattedLog << "\n";
	}
}

bool AbstractLoggerOutput::FlushNow() {
	if (queue.Size() >= logConfig.flushAfter) {
		return true;
	}
	return false;
}
