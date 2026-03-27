#pragma once
#include "Window.h"

class App
{
private:
    Window window;

public:
    App();
    int Go();

private:
    void DoFrame();
};
