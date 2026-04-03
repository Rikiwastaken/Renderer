#pragma once
#include <ctime>

class DeltaTimeCalculator
{
private:
    float lastframetime = 0.0f;    // Variable to hold the time of the last frame
    float frametimes[60] = {0.0f}; // Array to hold the time of the last 60 frames for averaging
    int framerate;

public:
    DeltaTimeCalculator() = default;
    ~DeltaTimeCalculator() = default;
    float GetDeltaTime() noexcept;     // Function to calculate and return the delta time (time elapsed since the last frame) based on the time from the last frame for the last 10 frames
    int GetFramerate() const noexcept; // Function to get the current frame rate (calculated as the inverse of the average delta time)
};
