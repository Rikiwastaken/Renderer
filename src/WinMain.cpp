
#define UNICODE
#define _UNICODE
#include <iostream>

#include "WindowsMessageMap.h"
#include "Window.h"

using namespace std;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) // Window procedure to handle messages for the window
{
    static WindowsMessageMap messageMap;                       // Create an instance of the WindowsMessageMap to convert messages to strings
    std::cout << messageMap(msg, lParam, wParam) << std::endl; // Print the message information to the console
    switch (msg)
    {
    case WM_CREATE:
        std::cout << "WM_CREATE" << std::endl;
        break;

    case WM_CLOSE:
        std::cout << "WM_CLOSE" << std::endl;
        // Destroy the window — triggers WM_DESTROY
        DestroyWindow(hWnd);
        return 0; // Prevent DefWindowProc from destroying again

    case WM_DESTROY:
        std::cout << "WM_DESTROY" << std::endl;
        PostQuitMessage(0); // End message loop
        return 0;

    default:
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam); // Default message handling
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    std::cout << "Debug output visible in console!" << std::endl;

    cout << "Creating window..." << endl;

    Window wnd(800, 600, "My Cool Window");

    // Message loop
    MSG msg;
    int result;
    while ((result = GetMessage(&msg, nullptr, 0, 0)) > 0) // GetMessage returns > 0 if a message is retrieved, 0 if WM_QUIT is received, and < 0 on error
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