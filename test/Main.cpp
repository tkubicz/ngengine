#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"

namespace l = NGE::Tools::Logger;

int main(int argc, char** argv) {
	l::NewLogger& log = l::NewLogger::GetInstance();
	log.SetEnabled(false);

	int result = Catch::Session().run(argc, argv);

	return result;
}