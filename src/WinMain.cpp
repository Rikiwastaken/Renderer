#define UNICODE
#define _UNICODE
#include <iostream>
#include <Windows.h>
#include "WindowsMessageMap.h"

using namespace std;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) // Window procedure to handle messages for the window
{
    static WindowsMessageMap messageMap;                                  // Create an instance of the WindowsMessageMap to convert messages to strings
    OutputDebugStringA((messageMap(msg, lParam, wParam) + "\n").c_str()); // Output the message information to the debug console
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(7); // Post a quit message to exit the message loop
        DestroyWindow(hWnd);
        break;

    default:
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam); // Default message handling
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    cout << "Creating window..." << endl;

    LPCWSTR pClassName = L"CoolWindowClass";

    // register Window Class
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(wc);        // Set the size of the structure
    wc.style = CS_OWNDC;           // Class style
    wc.lpfnWndProc = WndProc;      // Window procedure
    wc.cbClsExtra = 0;             // Extra bytes after the window class
    wc.cbWndExtra = 0;             // Extra bytes after the window instance
    wc.hInstance = hInstance;      // Handle to the instance
    wc.hIcon = nullptr;            // Handle to the class icon
    wc.hCursor = nullptr;          // Handle to the class cursor
    wc.hbrBackground = nullptr;    // Handle to the class background brush
    wc.lpszMenuName = nullptr;     // Resource name of the class menu
    wc.lpszClassName = pClassName; // Name of the window class

    RegisterClassEx(&wc);

    // create Window
    HWND hWnd = CreateWindowEx(
        0,                                                                         // Optional window styles
        pClassName,                                                                // Window class name
        L"Cool Window",                                                            // Window title
        WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_SYSMENU, // Window style

        // Size and position
        200, 200, 640, 480,

        nullptr,   // Parent window
        nullptr,   // Menu
        hInstance, // Instance handle
        nullptr    // Additional application data
    );

    // Show the window

    ShowWindow(hWnd, SW_SHOW);

    // Message loop
    MSG msg;
    BOOL result;
    while (result = GetMessage(&msg, nullptr, 0, 0) > 0) // GetMessage returns > 0 if a message is retrieved, 0 if WM_QUIT is received, and < 0 on error
    {
        TranslateMessage(&msg); // Translates virtual-key messages into character messages
        DispatchMessage(&msg);  // Dispatches a message to a window procedure
    }
    if (result == -1) // Check for errors in GetMessage
    {
        return -1; // Return -1 to indicate an error occurred
    }
    else
    {
        return msg.wParam; // Return the exit code from the WM_QUIT message
    }
}