#pragma once
#include "Window.h"

class App
{
private:
    Window window;
    std::vector<std::unique_ptr<class Box>> boxes; // Vector to hold unique pointers to Box objects, representing the drawable objects in the application
public:
    App();
    int Go();
    ~App();

private:
    void DoFrame();
};
