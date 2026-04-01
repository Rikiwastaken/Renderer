#include "DeltaTimeCalculator.h"

float DeltaTimeCalculator::GetDeltaTime() noexcept
{
    float time = clock() / static_cast<float>(CLOCKS_PER_SEC); // Get the current time
    float deltaTime = time - lastframetime;                    // Calculate the time elapsed since the last frame
    lastframetime = time;                                      // Update the last frame time to the current time
    return deltaTime;                                          // Return the calculated delta time
}