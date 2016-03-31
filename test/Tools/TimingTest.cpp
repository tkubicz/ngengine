#include <iostream>
#include <chrono>
#include "catch.hpp"
#include "NGE/Tools/Timing.hpp"

namespace t = NGE::Tools;
namespace ch = std::chrono;

void verifyTimeDefaultFormat(const std::string& timeInFormat, const std::tm& localTime) {
	int year = std::stoi(timeInFormat.substr(0, 4));
	REQUIRE((year - 1900) == localTime.tm_year);

	int month = std::stoi(timeInFormat.substr(5, 2));
	REQUIRE((month - 1) == localTime.tm_mon);

	int mday = std::stoi(timeInFormat.substr(8, 2));
	REQUIRE(mday == localTime.tm_mday);

	int hour = std::stoi(timeInFormat.substr(11, 2));
	REQUIRE(hour == localTime.tm_hour);

	int minute = std::stoi(timeInFormat.substr(14, 2));
	REQUIRE(minute == localTime.tm_min);

	int second = std::stoi(timeInFormat.substr(17, 2));
	REQUIRE(second == localTime.tm_sec);

	int milli = std::stoi(timeInFormat.substr(20, 3));
	REQUIRE((milli >= 0 && milli <= 999));
}

void verivyTimeInDDMMYYFormat(const std::string& timeInFormat, const std::tm& localTime) {
	int mday = std::stoi(timeInFormat.substr(0, 2));
	REQUIRE(mday == localTime.tm_mday);

	int month = std::stoi(timeInFormat.substr(4, 2));
	REQUIRE((month - 1) == localTime.tm_mon);

	int year = std::stoi(timeInFormat.substr(6, 2));
	REQUIRE((year + 100) == localTime.tm_year);
}

SCENARIO("Get time in format", "[timing]") {

	GIVEN("Instance of the timing class") {
		t::Timing& time = t::Timing::GetInstance();

		ch::high_resolution_clock::time_point timePoint = ch::high_resolution_clock::now();
		auto milliseconds = ch::duration_cast<ch::milliseconds>(timePoint.time_since_epoch());
		auto seconds = ch::duration_cast<ch::seconds>(timePoint.time_since_epoch());
		std::time_t currentTime = seconds.count();
		std::tm localTime = *std::localtime(&currentTime);

		WHEN("Get current time in default format %Y-%m-%d %H:%M:%S.%f") {
			std::string currentTimeInDefaultFormat = time.GetCurrentTimeInFormat();

			THEN("Time should be in YYYY-mm-dd HH:MM:SS.fff") {
				verifyTimeDefaultFormat(currentTimeInDefaultFormat, localTime);
			}
		}

		WHEN("Get current time in specified format %d-%m-%y") {
			std::string currentTimeInFormat = time.GetCurrentTimeInFormat("%d-%m-%y");

			THEN("Time should be in dd-mm-yy") {
				verivyTimeInDDMMYYFormat(currentTimeInFormat, localTime);
			}
		}

		WHEN("Get time from milliseconds in default format %Y-%m-%d %H:%M:%S.%f") {
			std::string timeInFormat = time.GetTimeInFormat(milliseconds);

			THEN("Time should be in YYYY-mm-dd HH:MM:SS.fff") {
				verifyTimeDefaultFormat(timeInFormat, localTime);
			}
		}

		WHEN("Get time from milliseconds in specified format %d-%m-%y") {
			std::string timeInFormat = time.GetTimeInFormat(milliseconds, "%d-%m-%y");

			THEN("Time should be in dd-mm-yy") {
				verivyTimeInDDMMYYFormat(timeInFormat, localTime);
			}
		}
	}
}

SCENARIO("Get current time in milliseconds", "[timing]") {

	GIVEN("Instance of the timing class and current time from chrono") {
		t::Timing& time = t::Timing::GetInstance();
		ch::high_resolution_clock::time_point timePoint = ch::high_resolution_clock::now();
		unsigned long millis = ch::duration_cast<ch::milliseconds>(timePoint.time_since_epoch()).count();
		unsigned long millisFromTiming = time.GetCurrentTimeInMs().count();

		THEN("Current time from Timing is equal time from chrono or slightly bigger") {
			REQUIRE((millisFromTiming >= millis && millisFromTiming <= millis + 500));
		}
	}
}

SCENARIO("Using Timing class", "[timing]") {

	GIVEN("Uninitialised Timing class") {
		t::Timing& time = t::Timing::GetInstance();
		time.Deinitialize();

		WHEN("Timing class is initialised and updated") {
			time.Initialize();
			time.Update();

			THEN("Get last frame duration. Should be bigger than zero") {
				REQUIRE((time.GetLastFrameDuration() > 0));
			}
		}

		WHEN("Timing class is updated without initialisation") {
			time.Update();

			THEN("Try to get last frame duration. It should be zero") {
				REQUIRE(time.GetLastFrameDuration() == 0);
			}
		}
	}
}