#include <iostream>
#include <Windows.h>
using namespace std;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    cout << "Creating window..." << endl;

    const auto pClassName = "CoolWindowClass";

    // register Window Class
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(wc);         // Set the size of the structure
    wc.style = CS_OWNDC;            // Class style
    wc.lpfnWndProc = DefWindowProc; // Window procedure
    wc.cbClsExtra = 0;              // Extra bytes after the window class
    wc.cbWndExtra = 0;              // Extra bytes after the window instance
    wc.hInstance = hInstance;       // Handle to the instance
    wc.hIcon = nullptr;             // Handle to the class icon
    wc.hCursor = nullptr;           // Handle to the class cursor
    wc.hbrBackground = nullptr;     // Handle to the class background brush
    wc.lpszMenuName = nullptr;      // Resource name of the class menu
    wc.lpszClassName = pClassName;  // Name of the window class

    RegisterClassEx(&wc);

    // create Window
    HWND hWnd = CreateWindowEx(
        0,                                        // Optional window styles
        pClassName,                               // Window class name
        "Cool Window",                            // Window title
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // Window style

        // Size and position
        200, 200, 640, 480,

        nullptr,   // Parent window
        nullptr,   // Menu
        hInstance, // Instance handle
        nullptr    // Additional application data
    );

    // Show the window

    ShowWindow(hWnd, SW_SHOW);

    while (true)
        ;

    return 0;
}