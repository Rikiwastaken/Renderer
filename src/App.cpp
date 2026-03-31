#include "App.h"
#include <iostream>

using namespace std;

float angle = 0.0f; //   Global variable to track the rotation angle for the test triangle (used in DrawTestTriangle function)

float width = 1000.0f;  // Global variable to track the width of the window (used in DrawTestTriangle function)
float height = 1000.0f; // Global variable to track the height of the window (used in DrawTestTriangle function)
bool goingright = true;
bool goingup = true;
bool goingforward = true;
float currentx = 0.0f;
float currenty = 0.0f;
float currentz = 0.0f;

App::App() : window(width, height, "Riki Engine") // Initialize the window with width, height, and title
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
    // window.GetGraphics().DrawTestCube(-angle / 5, std::sin(angle), 0.0f, 4.0f + std::cos(angle), width, height);

    if (currentx == 0.0f && currenty == 0.0f && currentz == 0.0f)
    {
        float random1 = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f; // Generate a random float between -1.0 and 1.0 for the x coordinate
        float random2 = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f; // Generate a random float between -1.0 and 1.0 for the y coordinate
        currentx = random1 * (width / 2 - 100) / width;                        // Set the initial x coordinate based on the random value and window width
        currenty = random2 * (height / 2 - 100) / height;
    }

    angle += 0.1f; // Increment the rotation angle for the test triangle (this will cause the triangle to rotate over time)
    cout << "angle: " << angle << endl;

    float moveperframe = 5.0f;
    float offsetx = -0.3f * width;
    float offsety = -0.1f * height;
    if (goingright)
    {
        currentx += (moveperframe / width);
    }
    else
    {
        currentx -= moveperframe / width;
    }
    if (currentx > (width / 2 - offsetx) / width)
    {
        goingright = false;
    }
    else if (currentx < (-width / 2 + offsetx) / width)
    {
        goingright = true;
    }

    if (goingup)
    {
        currenty += moveperframe / height;
    }
    else
    {
        currenty -= moveperframe / height;
    }

    if (currenty > (height / 2 - offsety) / height)
    {
        goingup = false;
    }
    else if (currenty < (-height / 2 + offsety) / height)
    {
        goingup = true;
    }

    if (goingforward)
    {
        currentz += moveperframe;
    }
    else
    {
        currentz -= moveperframe;
    }

    window.GetGraphics().DrawTestCube(angle / 5, currentx, currenty, 1, 0.1f, width, height);

    window.GetGraphics().EndFrame();
}