
#define UNICODE
#define _UNICODE
#include <iostream>

#include "WindowsMessageMap.h"
#include "Window.h"
#include <string>
#include "App.h"

using namespace std;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    try
    {
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        std::cout << "Debug output visible in console!" << std::endl;

        cout << "Creating window..." << endl;
        return App{}.Go();
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