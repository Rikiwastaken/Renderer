#include "App.h"
#include <iostream>
using namespace std;

int angle = 0.0f; // Global variable to track the rotation angle for the test triangle (used in DrawTestTriangle function)

App::App() : window(800, 600, "Riki Engine") // Initialize the window with width, height, and title
{
}

int App::Go()
{
    while (true)
    {
        if (const auto exitCode = Window::ProcessMessages()) // Process Windows messages and check if the window has been closed
        {
            return *exitCode; // If the window is closed, return the exit code
        }
        DoFrame(); // Perform per-frame updates (e.g., rendering, game logic, etc.)
    }
}

void App::DoFrame()
{
    window.GetGraphics().ClearBuffer(0.0f, 0.2f, 0.4f); // Clear the back buffer with a specified color (e.g., a shade of blue)
    window.GetGraphics().DrawTestTriangle(angle);       // Draw a test triangle (implementation not shown here)
    angle += 1.0f;                                      // Increment the rotation angle for the test triangle (this will cause the triangle to rotate over time)
    cout << "angle: " << angle << endl;
    window.GetGraphics().EndFrame();
    // Placeholder for per-frame updates (e.g., rendering, game logic, etc.)
}