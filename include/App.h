#pragma once
#include "Window.h"
#include "Drawable/Sheet.h"
#include "imgui/Imguimanager.h"

class App
{
private:
    Window window;
    std::vector<std::unique_ptr<class Drawable>> drawables;
    static constexpr int nDrawables = 100;
    ImGuiManager imguimanager;

public:
    App();
    int Go();
    ~App();

private:
    void DoFrame();
};
