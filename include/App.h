#pragma once
#include "Window.h"
#include "Drawable/Sheet.h"
#include "imgui/Imguimanager.h"
#include "Camera.h"

class App
{
private:
    Window window;
    std::vector<std::unique_ptr<class Drawable>> drawables;
    static constexpr int nDrawables = 100;
    ImGuiManager imguimanager;
    float speed_facotr = 1.0f;
    Camera camera;

public:
    App();
    int Go();
    ~App();

private:
    void DoFrame();
};
