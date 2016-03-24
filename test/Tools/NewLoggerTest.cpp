#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"
#include "NGE/Tools/Logger/Output/StreamLoggerOutput.hpp"

namespace l = NGE::Tools::Logger;
namespace o = l::Output;

TEST_CASE("Logger can log at different levels", "[logger]") {
	l::NewLogger& log = l::NewLogger::GetInstance();
	log.GetOutputs()["console"]->SetEnabled(false);
	log.GetOutputs()["file"]->SetEnabled(false);
}

TEST_CASE("Logger can be extended by a new output", "[logger]") {
	l::NewLogger& log = l::NewLogger::GetInstance();
	log.Initialise();
	std::stringstream ss;

	unsigned int defaultOutputSize = log.GetOutputs().size();
	o::StreamLoggerOutput* streamLogger = new o::StreamLoggerOutput(log.GetLogLevel(), log.GetLogFormat(), log.GetDateFormat());
	streamLogger->SetOutputStream(&ss);

	// Disable other outputs.
	for (auto& kv : log.GetOutputs()) {
		kv.second->SetEnabled(false);
	}

	log.GetOutputs().insert(std::pair<std::string, o::AbstractLoggerOutput*>("custom_stream", streamLogger));
	REQUIRE(log.GetOutputs().size() == defaultOutputSize + 1);

	// Log some messages.
	log_trace("trace");
	log_debug("debug");
	log_info("info");
	log_warn("warn");
	log_error("error");
	log_critical("critical");

	// Check internal queue.
	REQUIRE(streamLogger->GetQueue().Size() == 6);

	// Flush logs from internal queue to stream.
	log.Flush();

	std::string result = ss.str();
	REQUIRE(!result.empty());

	std::cout << result << std::endl;
}

TEST_CASE("Check logger default configuration") {
	l::NewLogger& log = l::NewLogger::GetInstance();
	log.Initialise();

	REQUIRE(log.GetDateFormat() == "%Y-%m-%d %H:%M:%S.%f");
	REQUIRE(log.GetLogFormat() == "[{date}][{level}][{shortFile}/{shortFunction}[{line}]] - {log}");
	REQUIRE(log.GetLogLevel() == l::LogTypes::LOG_LEVEL::TRACE);
	REQUIRE(log.IsAutoFlushEnabled() == true);
	REQUIRE(log.GetFlushAfter() == 20);
}

SCENARIO("Logger can write to multiple outputs", "[logger]") {

	GIVEN("A Logger with default configuration") {
		l::NewLogger& log = l::NewLogger::GetInstance();
		REQUIRE(log.GetOutputs().size() == 2);
	}
}

SCENARIO("Logger can write to different outputs on different levels", "[logger]") {

	GIVEN("A Logger with three stream configurations") {

	}
}