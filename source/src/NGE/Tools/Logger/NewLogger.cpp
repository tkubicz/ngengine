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
	outputs.insert(std::pair<std::string, Output::LoggerOutput*>("file", new Output::FileLoggerOutput(logLevel, logFormat, dateFormat, flushAfter, true)));
	outputs.insert(std::pair<std::string, Output::LoggerOutput*>("console", new Output::ConsoleLoggerOutput(logLevel, logFormat, dateFormat, flushAfter, true)));
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
	logLevel = LogTypes::LOG_LEVEL::TRACE;
	flushAfter = 20;
	logFormat = "[{date}][{level}][{shortFile}/{shortFunction}[{line}]] - {log}";
	dateFormat = "%Y-%m-%d %H:%M:%S.%f";

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
	return logLevel;
}

void NewLogger::SetLogLevel(LogTypes::LOG_LEVEL logLevel) {
	this->logLevel = logLevel;
	for (auto& kv : outputs) {
		kv.second->SetLogLevel(logLevel);
	}
}

std::string NewLogger::GetLogFormat() const {
	return logFormat;
}

void NewLogger::SetLogFormat(const std::string& logFormat) {
	this->logFormat = logFormat;
	for (auto& kv : outputs) {
		kv.second->SetLogFormat(logFormat);
	}
}

std::string NewLogger::GetDateFormat() const {
	return dateFormat;
}

void NewLogger::SetDateFormat(const std::string& dateFormat) {
	this->dateFormat = dateFormat;
	for (auto& kv : outputs) {
		kv.second->SetDateFormat(dateFormat);
	}
}

std::map<std::string, Output::LoggerOutput*>& NewLogger::GetOutputs() {
	return outputs;
}
