
#define UNICODE
#define _UNICODE
#include <iostream>

#include "WindowsMessageMap.h"
#include "Window.h"
#include <string>

using namespace std;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    try
    {
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        std::cout << "Debug output visible in console!" << std::endl;

        cout << "Creating window..." << endl;

        Window window(800, 600, "My Cool Window");

        // Message loop
        MSG msg;
        int result;
        while ((result = GetMessage(&msg, nullptr, 0, 0)) > 0) // GetMessage returns > 0 if a message is retrieved, 0 if WM_QUIT is received, and < 0 on error
        {
            TranslateMessage(&msg);                      // Translates virtual-key messages into character messages
            DispatchMessage(&msg);                       // Dispatches a message to a window procedure
            if (window.keyboard.KeyIsPressed(VK_ESCAPE)) // Check if the Escape key is pressed to exit the application
            {
                PostQuitMessage(0); // Post a WM_QUIT message to end the message loop
            }
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
    catch (const RikiException &e)
    {
        MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONERROR);
        return -1;
    }
    catch (const std::exception &e)
    {
        MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONERROR);
        return -1;
    }
    catch (...)
    {
        MessageBoxW(nullptr, L"An unknown error occurred.", L"Unknown Exception", MB_OK | MB_ICONERROR);
        return -1;
    };
}