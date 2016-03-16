#include <iostream>
#include <chrono>
#include <iomanip>
#include "NGE/Tools/Timing.hpp"

using namespace NGE::Tools;

void Timing::Initialize() {

	frameNumber = 0;

	lastFrameTimestamp = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
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

	double thisTime = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
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
	std::time_t currentTime = std::time(NULL);
	if (std::strftime(&timeBuffer[0], timeBufferSize, format.c_str(), std::localtime(&currentTime))) {
		return std::string(&timeBuffer[0]);
	}
	return NULL;
}
