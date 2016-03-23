#include <iostream>
#include <iomanip>
#include <c++/4.9/bits/stringfwd.h>
#include "NGE/Tools/Timing.hpp"
#include "NGE/Parsers/StringUtils.hpp"
#include "cppformat/format.h"

using namespace NGE::Tools;
namespace ch = std::chrono;

void Timing::Initialize() {

	frameNumber = 0;

	lastFrameTimestamp = ch::duration_cast<ch::duration<double>>(ch::high_resolution_clock::now().time_since_epoch()).count();
	lastFrameDuration = 0;

	isPaused = false;

	averageFrameDuration = 0;
	fps = 0;
}

void Timing::Deinitialize() {
}

void Timing::Update() {
	if (!isPaused) {
		frameNumber++;
	}

	double thisTime = ch::duration_cast<ch::duration<double>>(ch::high_resolution_clock::now().time_since_epoch()).count();
	lastFrameDuration = thisTime - lastFrameTimestamp;
	lastFrameTimestamp = thisTime;

	if (frameNumber > 1) {
		if (averageFrameDuration <= 0)
			averageFrameDuration = (double) lastFrameDuration;
		else {
			averageFrameDuration *= 0.99;
			averageFrameDuration += 0.01 * (double) lastFrameDuration;

			fps = (float) (1000.0 / averageFrameDuration);
		}
	}
}

std::string Timing::GetCurrentTimeInFormat(const std::string& format) {
	ch::high_resolution_clock::time_point timePoint = ch::high_resolution_clock::now();
	auto milliseconds = ch::duration_cast<ch::milliseconds>(timePoint.time_since_epoch());
	return GetTimeInFormat(milliseconds, format);
}

std::string Timing::GetTimeInFormat(const ch::milliseconds& milliseconds, const std::string& format) {
	auto seconds = ch::duration_cast<ch::seconds>(milliseconds);
	std::time_t currentTime = seconds.count();
	std::tm localTime = *std::localtime(&currentTime);

	std::string replacedFormat = format;
	NGE::Parsers::StringUtils::ReplaceAll(replacedFormat, "%f", std::to_string(milliseconds.count() % 1000));

	if (std::strftime(&timeBuffer[0], timeBufferSize, replacedFormat.c_str(), &localTime)) {
		return std::string(&timeBuffer[0]);
	}

	return std::string();
}

std::chrono::milliseconds Timing::GetCurrentTimeInMs() {
	ch::high_resolution_clock::time_point timePoint = ch::high_resolution_clock::now();
	return ch::duration_cast<ch::milliseconds>(timePoint.time_since_epoch());
}
