#include "NGE/Tools/Logger/NewLogger.hpp"
#include "NGE/Tools/Logger/Output/FileLoggerOutput.hpp"
#include "NGE/Tools/Logger/Output/ConsoleLoggerOutput.hpp"
using namespace NGE::Tools::Logger;

NewLogger::NewLogger() : Singleton() {
	Initialise();
}

NewLogger::~NewLogger() {
	ClearOutputs();
}

void NewLogger::InitialiseDefaultOutputs() {
	outputs.insert(OutputPair("file", new Output::FileLoggerOutput(logConfig)));
	outputs.insert(OutputPair("console", new Output::ConsoleLoggerOutput(logConfig)));
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

Output::AbstractLoggerOutput* NewLogger::operator[](const std::string& name) {
	auto findOutput = outputs.find(name);
	if (findOutput == outputs.end()) {
		log_warn("Could not find logger output: '{}'", name);

		return nullptr;
	}
	return findOutput->second;
}

void NewLogger::Initialise() {
	logConfig.enabled = true;
	logConfig.logFormat = "[{date}][{level}][{shortFile}/{shortFunction}[{line}]] - {log}";
	logConfig.dateFormat = "%Y-%m-%d %H:%M:%S.%f";
	logConfig.logLevel = LogTypes::LOG_LEVEL::TRACE;
	logConfig.autoFlushEnabled = true;
	logConfig.flushAfter = 20;

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

bool NewLogger::IsEnabled() const {
	return logConfig.enabled;
}

void NewLogger::SetEnabled(bool enabled) {
	this->logConfig.enabled = enabled;
	for (auto& kv : outputs) {
		kv.second->SetEnabled(enabled);
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

OutputMap& NewLogger::GetOutputs() {
	return outputs;
}

/**
 * TODO: Finish implementation.
 * @param xmlFilePath
 * @return 
 */
bool NewLogger::LoadXMLSettings(const std::string& xmlFilePath) {
	return true;
}

/**
 * TODO: Finish implementation.
 * @param xmlFilePath
 * @return 
 */
bool NewLogger::SaveXMLSettings(const std::string& xmlFilePath) {
	return true;
}


