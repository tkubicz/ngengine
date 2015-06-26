#include <iostream>
#include <fstream>

#include "NGE/Tools/Logger.hpp"

using namespace NGE::Tools;

std::vector<std::string> Logger::logStrings;
std::string Logger::logPath;
bool Logger::showOutput;

void Logger::Initialize(const std::string &filename, bool output) {
	showOutput = output;
	logPath = (filename.empty() ? "log.txt" : filename);
	std::ofstream logFile(logPath.c_str());
	logFile.close();
}

void Logger::Flush() {
	if (!logPath.length() || !logStrings.size())
		return;

	std::ofstream logFile(logPath.c_str(), std::ios::app);

	for (size_t t = 0; t < logStrings.size(); ++t)
		logFile << logStrings[t];

	logStrings.clear();
	logFile.clear();
}

void Logger::WriteImmidiatleInfoLog(const std::string& info) {
	if (info.length()) {
		logStrings.push_back("<+> " + info + "\n");
		ShowOutput();

		Flush();
	}
}

void Logger::WriteFatalErrorLog(const std::string& error) {
	if (error.length()) {
		logStrings.push_back("<X> " + error + "\n");
		ShowOutput();

		Flush();
	}
}

void Logger::WriteInfoLog(const std::string& info) {
	logStrings.push_back("<+> " + info + "\n");
	ShowOutput();

	if (logStrings.size() >= 10)
		Flush();
}

void Logger::WriteErrorLog(const std::string& error) {
	if (error.length()) {
		logStrings.push_back("<!> " + error + "\n");
		ShowOutput();
		Flush();
	}
}

void Logger::ShowOutput() {
	if (showOutput)
#ifdef ANDROID
		LOGD(logStrings.back().c_str());
#else
		std::cout << logStrings.back();
#endif
}