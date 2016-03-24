#include "NGE/Tools/Logger/NewLogger.hpp"
#include "NGE/Tools/Logger/Output/FileLoggerOutput.hpp"
#include "NGE/Tools/Logger/Output/ConsoleLoggerOutput.hpp"
using namespace NGE::Tools::Logger;

NewLogger::NewLogger() {
	Initialise();
}

NewLogger::~NewLogger() {
	ClearOutputs();
}

void NewLogger::InitialiseDefaultOutputs() {
	outputs.insert(std::pair<std::string, Output::AbstractLoggerOutput*>("file", new Output::FileLoggerOutput(logConfig)));
	outputs.insert(std::pair<std::string, Output::AbstractLoggerOutput*>("console", new Output::ConsoleLoggerOutput(logConfig)));
}

bool NewLogger::CheckLogMessageLevel(LogTypes::LOG_LEVEL logLevel, LogTypes::LOG_LEVEL loggerLevel) {
	if (logLevel < loggerLevel) {
		return false;
	}
	return true;
}

void NewLogger::ClearOutputs() {
	for (auto& kv : outputs) {
		if (kv.second != nullptr) {
			delete kv.second;
		}
	}
	outputs.clear();
}

NewLogger& NewLogger::GetInstance() {
	static NewLogger instance;

	return instance;
}

void NewLogger::Initialise() {
	logConfig.logLevel = LogTypes::LOG_LEVEL::TRACE;
	logConfig.autoFlushEnabled = true;
	logConfig.flushAfter = 20;
	logConfig.logFormat = "[{date}][{level}][{shortFile}/{shortFunction}[{line}]] - {log}";
	logConfig.dateFormat = "%Y-%m-%d %H:%M:%S.%f";

	ClearOutputs();
	InitialiseDefaultOutputs();
}

void NewLogger::Flush() {
	for (auto& kv : outputs) {
		if (kv.second->IsEnabled()) {
			kv.second->Flush();
		}
	}
}

LogTypes::LOG_LEVEL NewLogger::GetLogLevel() const {
	return logConfig.logLevel;
}

void NewLogger::SetLogLevel(LogTypes::LOG_LEVEL logLevel) {
	this->logConfig.logLevel = logLevel;
	for (auto& kv : outputs) {
		kv.second->SetLogLevel(logLevel);
	}
}

std::string NewLogger::GetLogFormat() const {
	return logConfig.logFormat;
}

void NewLogger::SetLogFormat(const std::string& logFormat) {
	this->logConfig.logFormat = logFormat;
	for (auto& kv : outputs) {
		kv.second->SetLogFormat(logFormat);
	}
}

std::string NewLogger::GetDateFormat() const {
	return logConfig.dateFormat;
}

void NewLogger::SetDateFormat(const std::string& dateFormat) {
	this->logConfig.dateFormat = dateFormat;
	for (auto& kv : outputs) {
		kv.second->SetDateFormat(dateFormat);
	}
}

LogConfig NewLogger::GetLogConfig() const {
	return logConfig;
}

void NewLogger::SetLogConfig(LogConfig logConfig) {
	this->logConfig = logConfig;
	for (auto& kv : outputs) {
		kv.second->SetLogConfig(logConfig);
	}
}

bool NewLogger::IsAutoFlushEnabled() const {
	return logConfig.autoFlushEnabled;
}

void NewLogger::SetAutoFlushEnabled(bool autoFlushEnabled) {
	this->logConfig.autoFlushEnabled = autoFlushEnabled;
	for (auto& kv : outputs) {
		kv.second->SetAutoFlushEnabled(autoFlushEnabled);
	}
}

unsigned int NewLogger::GetFlushAfter() const {
	return this->logConfig.flushAfter;
}

void NewLogger::SetFlushAfter(unsigned int flushAfter) {
	this->logConfig.flushAfter = flushAfter;
	for (auto& kv : outputs) {
		kv.second->SetFlushAfter(flushAfter);
	}
}

std::map<std::string, Output::AbstractLoggerOutput*>& NewLogger::GetOutputs() {
	return outputs;
}
