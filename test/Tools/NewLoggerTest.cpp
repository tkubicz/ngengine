#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"
#include "NGE/Tools/Logger/Output/StreamLoggerOutput.hpp"

namespace l = NGE::Tools::Logger;
namespace o = l::Output;

TEST_CASE("Logger can log at different levels", "[logger]") {
	l::NewLogger& log = l::NewLogger::GetInstance();

	SECTION("log trace") {
		log_trace("Trace log");
	}

	SECTION("log debug") {
		log_debug("Debug log");
	}

	SECTION("log info") {
		log_info("Info log");
	}

	SECTION("log warn") {
		log_warn("Warn log");
	}

	SECTION("log error") {
		log_error("Error log");
	}

	SECTION("log critical error") {
		log_critical("Critical log");
	}
}

TEST_CASE("Logger can be extended by a new stream", "[logger]") {
	l::NewLogger& log = l::NewLogger::GetInstance();
	unsigned int defaultOutputSize = log.GetOutputs().size();

	log.GetOutputs().insert(std::pair<std::string, o::AbstractLoggerOutput*>("custom_stream", new o::StreamLoggerOutput(log.GetLogLevel(), log.GetLogFormat(), log.GetDateFormat(), 20, true)));
	REQUIRE(log.GetOutputs().size() == defaultOutputSize + 1);

	o::StreamLoggerOutput* streamLoggerOutput = dynamic_cast<o::StreamLoggerOutput*> (log.GetOutputs()["custom_stream"]);
	REQUIRE(streamLoggerOutput != nullptr);

	streamLoggerOutput->SetOutputStream(&std::cout);

	REQUIRE(false);
}