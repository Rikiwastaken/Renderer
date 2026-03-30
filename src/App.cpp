#include "App.h"

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
    window.GetGraphics().DrawTestTriangle();            // Draw a test triangle (implementation not shown here)

    window.GetGraphics().EndFrame();
    // Placeholder for per-frame updates (e.g., rendering, game logic, etc.)
}