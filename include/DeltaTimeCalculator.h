#pragma once
#include <ctime>

class DeltaTimeCalculator
{
private:
    float lastframetime = 0.0f; // Variable to hold the time of the last frame

public:
    DeltaTimeCalculator() = default;
    ~DeltaTimeCalculator() = default;
    float GetDeltaTime() noexcept; // Function to calculate and return the delta time (time elapsed since the last frame) based on the time from the last frame for the last 10 frames
};
