#include "App.h"
#include <iostream>
#include "Drawable/Box.h"
#include <memory>
#include "DeltaTimeCalculator.h"
#include "Drawable/Melon.h"
#include "Drawable/Pyramid.h"
#include "Drawable/SkinnedBox.h"
#include "CustomMath.h"
#include "Surface.h"
#include "GdiPlusManager.h"
#include "imgui/imgui.h"

using namespace std;

namespace dx = DirectX;

GDIPlusManager gdiPlusManager; // Global instance of the GDIPlusManager to manage the initialization and cleanup of GDI+ for image loading and saving

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

App::App() : window(800, 600, "Riki Engine") // Initialize the window with width, height, and title
{
    class Factory
    {
    public:
        Factory(Graphics &gfx)
            : gfx(gfx)
        {
        }
        std::unique_ptr<Drawable> operator()()
        {
            switch (typedist(rng))
            {
            case 0:
                return std::make_unique<Sheet>(
                    gfx, rng, adist, ddist,
                    odist, rdist);

            case 1:
                return std::make_unique<SkinnedBox>(
                    gfx, rng, adist, ddist,
                    odist, rdist);
            case 2:
                return std::make_unique<Melon>(
                    gfx, rng, adist, ddist,
                    odist, rdist, longdist, latdist);
            case 3:
                return std::make_unique<Pyramid>(
                    gfx, rng, adist, ddist,
                    odist, rdist);
            default:
                assert(false && "bad drawable type in factory");
                return {};
            }
        }

    private:
        Graphics &gfx;
        std::mt19937 rng{std::random_device{}()};
        std::uniform_real_distribution<float> adist{0.0f, PI * 2.0f};
        std::uniform_real_distribution<float> ddist{0.0f, PI * 0.5f};
        std::uniform_real_distribution<float> odist{0.0f, PI * 0.08f};
        std::uniform_real_distribution<float> rdist{6.0f, 20.0f};
        std::uniform_real_distribution<float> bdist{0.4f, 3.0f};
        std::uniform_int_distribution<int> latdist{5, 20};
        std::uniform_int_distribution<int> longdist{10, 40};
        std::uniform_int_distribution<int> typedist{0, 3};
    };

    Factory f(window.GetGraphics());
    drawables.reserve(nDrawables);
    std::generate_n(std::back_inserter(drawables), nDrawables, f);

    window.GetGraphics().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
    window.GetGraphics().SetCamera(DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f));
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
    window.GetGraphics().BeginFrame(1.0f, 1.0f, 1.0f);  // Clear the back buffer with a specified color (e.g., a shade of blue)
    window.GetGraphics().SetCamera(camera.GetMatrix()); // Set the camera view matrix for rendering (e.g., to control the perspective and orientation of the scene)
    float dt = dtc.GetDeltaTime() * speed_facotr;       // Get the delta time for the current frame (time elapsed since the last frame)
    for (auto &drawable : drawables)
    {
        drawable->Update(window.keyboard.KeyIsPressed(VK_SPACE) ? 0.0f : dt); // Update each Drawable object with the calculated delta time (e.g., update their transformations, animations, etc.)
        drawable->Draw(window.GetGraphics());                                 // Draw each Drawable  object using the graphics context
    }

    // imgui window to control speed factor
    if (ImGui::Begin("Control Panel"))
    {
        ImGui::SliderFloat("Speed Factor", &speed_facotr, 0.01f, 5.0f);                                                            // Slider to control the speed factor for the animation
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); // Display the average frame time and frames per second (FPS) in the ImGui window
    }
    ImGui::End();

    camera.SpawnControlWindow(); // Spawn a control window for the camera (e.g., to allow user input for controlling the camera's position and orientation)
    window.GetGraphics().EndFrame();
}