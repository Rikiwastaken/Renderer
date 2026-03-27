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
    window.SetTitle("Riki Engine - Frame Update" + std::to_string(rand() % 100));
    // Placeholder for per-frame updates (e.g., rendering, game logic, etc.)
}