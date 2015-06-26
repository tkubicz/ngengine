#include <iostream>
#include <chrono>
#include "NGE/Tools/Timing.hpp"

using namespace NGE::Tools;

static Timing* timingData = NULL;

void Timing::Initialize() {
	if (!timingData)
		timingData = new Timing();

	timingData->frameNumber = 0;

	//timingData->lastFrameTimestamp = glfwGetTime();
	timingData->lastFrameTimestamp = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	timingData->lastFrameDuration = 0;

	timingData->isPaused = false;

	timingData->averageFrameDuration = 0;
	timingData->fps = 0;
}

void Timing::Deinitialize() {
	delete timingData;
	timingData = NULL;
}

Timing& Timing::Get() {
	return (Timing&)*timingData;
}

void Timing::Update() {
	if (!timingData)
		return;

	if (!timingData->isPaused)
		timingData->frameNumber++;

	//double thisTime = glfwGetTime();
	double thisTime = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	timingData->lastFrameDuration = thisTime - timingData->lastFrameTimestamp;
	timingData->lastFrameTimestamp = thisTime;

	if (timingData->frameNumber > 1) {
		if (timingData->averageFrameDuration <= 0)
			timingData->averageFrameDuration = (double) timingData->lastFrameDuration;
		else {
			timingData->averageFrameDuration *= 0.99;
			timingData->averageFrameDuration += 0.01 * (double) timingData->lastFrameDuration;

			timingData->fps = (float) (1000.0 / timingData->averageFrameDuration);
		}
	}
}
