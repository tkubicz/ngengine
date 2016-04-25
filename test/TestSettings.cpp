#include "TestSettings.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"

namespace l = NGE::Tools::Logger;
l::LogConfig logConfig;

void enableConsoleLogging() {
	l::NewLogger& log = l::NewLogger::GetInstance();
	logConfig = log["console"]->GetLogConfig();
	log["console"]->SetEnabled(true);
	log["console"]->SetAutoFlushEnabled(true);
	log["console"]->SetFlushAfter(1);
}

void disableConsoleLogging() {
	l::NewLogger& log = l::NewLogger::GetInstance();
	log["console"]->SetLogConfig(logConfig);
}