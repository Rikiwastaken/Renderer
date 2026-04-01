#include "App.h"
#include <iostream>
#include "Drawable/Box.h"
#include <memory>
#include "DeltaTimeCalculator.h"

using namespace std;

float angle = 0.0f;      //   Global variable to track the rotation angle for the test triangle (used in DrawTestTriangle function)
DeltaTimeCalculator dtc; // Global instance of the DeltaTimeCalculator to calculate delta time for frame updates
float width = 800.0f;    // Global variable to track the width of the window (used in DrawTestTriangle function)
float height = 600.0f;   // Global variable to track the height of the window (used in DrawTestTriangle function)
bool goingright = true;
bool goingup = true;
bool goingforward = true;
float currentx = 0.0f;
float currenty = 0.0f;
float currentz = 0.0f;

App::App() : window(width, height, "Riki Engine") // Initialize the window with width, height, and title
{
std:
    mt19937 rng{std::random_device{}()};                                     // Create a random number generator seeded with a random value from the random device
    std::uniform_real_distribution<float> adist{0.0f, 2.0f * 3.1415926535f}; // Distribution for angles (0 to 2*pi)
    std::uniform_real_distribution<float> ddist{0.0f, 2.0f * 3.1415926535f}; // Distribution for angles (0 to 2*pi)
    std::uniform_real_distribution<float> odist{0.0f, 0.3f * 3.1415926535f}; // Distribution for offsets (0 to 0.3*pi)
    std::uniform_real_distribution<float> rdist{6.0f, 20.0f};                // Distribution for speeds
    for (auto i = 0; i < 100; i++)
    {
        boxes.push_back(std::make_unique<Box>(window.GetGraphics(), rng, adist, ddist, odist, rdist)); // Create 100 Box objects with random parameters and add them to the boxes vector
    }

    window.GetGraphics().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 1.0f, 0.5f, 40.0f)); // Set the projection matrix for the graphics context to a perspective projection with specified parameters
}

App::~App() {}; // Default destructor for the App class (no custom cleanup needed)

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
    /*
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

    // window.GetGraphics().DrawTestCube(angle / 5, currentx, currenty, 1, 0.1f, width, height);
    window.GetGraphics().DrawD10(angle / 10, currentx, currenty, 1, 0.1f, width, height);
    */

    float dt = dtc.GetDeltaTime();                      // Get the delta time for the current frame (time elapsed since the last frame)
    cout << "Delta Time: " << dt << " seconds" << endl; // Output the delta time to the console for debugging purposes
    for (auto &box : boxes)
    {
        box->Update(dt);                 // Update each Box object with the calculated delta time (e.g., update their transformations, animations, etc.)
        box->Draw(window.GetGraphics()); // Draw each Box object using the graphics context
    }

    window.GetGraphics().EndFrame();
}