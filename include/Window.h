#pragma once
#include "CustomWindows.h"

class Window
{
private:
    // singleton manage regirstation and cleanup of the window class
    class WindowClass
    {
    public:
        static const char *GetName() noexcept;
        static HINSTANCE GetInstance() noexcept;

    private:
        WindowClass() noexcept;
        ~WindowClass() noexcept;
        WindowClass(const WindowClass &) = delete;
        static constexpr const char *wndClassName = "Riki's Cool Window";
        static WindowClass wndClass;
        HINSTANCE hInst;
    };

public:
    Window(int width, int height, const char *name) noexcept; // constructor to create a window with the specified width, height, and name
    ~Window();                                                // destructor to destroy the window when the object goes out of scope
    Window(const Window &) = delete;                          // delete copy constructor to prevent copying of the window object
    Window &operator=(const Window &) = delete;               // delete copy assignment operator to prevent copying of the window object
private:
    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept; // static window procedure to set up the message handling for the window
    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept; // static window procedure to handle messages for the window after setup
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;                      // member function to handle messages for the window
private:
    HWND hWnd;  // handle to the window
    int width;  // width of the window
    int height; // height of the window
};