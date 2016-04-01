#include <iostream>
#include <iomanip>
#include "NGE/Tools/Timing.hpp"
#include "NGE/Parsers/StringUtils.hpp"
#include "cppformat/format.h"
#include "NGE/Tools/Logger/NewLogger.hpp"

using namespace NGE::Tools;
namespace ch = std::chrono;

Timing::Timing() {
    timeBufferSize = 100;
    timeBuffer = new char[timeBufferSize];
    initialised = false;
}

Timing::~Timing() {
    delete timeBuffer;
}

Timing& Timing::operator=(const Timing&) {
    return *this;
}

void Timing::SetInternalFieldsToDefault() {
    frameNumber = 0;
    lastFrameDuration = 0;
    paused = false;
    averageFrameDuration = 0;
    fps = 0;
}

void Timing::Initialize() {
    SetInternalFieldsToDefault();
    lastFrameTimestamp = ch::duration_cast<ch::duration<double>>(ch::high_resolution_clock::now().time_since_epoch()).count();
    initialised = true;
}

void Timing::Deinitialize() {
    SetInternalFieldsToDefault();
    lastFrameTimestamp = 0;
    initialised = false;
}

void Timing::Update() {
    if (!initialised) {
        log_warn("Trying to update time, but the timer is not initialised");
        return;
    }

    if (!paused) {
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

double Timing::GetLastFrameDuration() const {
    return lastFrameDuration;
}

double Timing::GetAverageFrameDuration() const {
    return averageFrameDuration;
}
