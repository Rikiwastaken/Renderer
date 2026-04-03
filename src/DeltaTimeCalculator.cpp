#include "DeltaTimeCalculator.h"

float DeltaTimeCalculator::GetDeltaTime() noexcept
{
    float time = clock() / static_cast<float>(CLOCKS_PER_SEC); // Get the current time
    float deltaTime = time - lastframetime;                    // Calculate the time elapsed since the last frame
    int length = sizeof(frametimes) / sizeof(frametimes[0]);
    lastframetime = time; // Update the last frame time to the current time
    // Shift the frame times in the array to make room for the new frame time
    for (int i = length - 1; i > 0; i--)
    {
        frametimes[i] = frametimes[i - 1]; // Shift the frame times to the right
    }
    frametimes[0] = deltaTime; // Store the new frame time at the beginning of the array
    return deltaTime;          // Return the calculated delta time
}

int DeltaTimeCalculator::GetFramerate() const noexcept
{
    float averageDeltaTime = 0.0f;                             // Variable to hold the average delta time
    float length = sizeof(frametimes) / sizeof(frametimes[0]); // Calculate the number of frame times stored in the array
    for (int i = 0; i < length; i++)
    {
        averageDeltaTime += frametimes[i]; // Sum the frame times in the array
    }
    averageDeltaTime /= length;                       // Calculate the average delta time
    return static_cast<int>(1.0f / averageDeltaTime); // Return the frame rate as the inverse of the average delta time
}