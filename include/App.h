#pragma once
#include "Window.h"
#include "Drawable/Sheet.h"

class App
{
private:
    Window window;
    std::vector<std::unique_ptr<class Drawable>> drawables;
    static constexpr int nDrawables = 100;

public:
    App();
    int Go();
    ~App();

private:
    void DoFrame();
};
