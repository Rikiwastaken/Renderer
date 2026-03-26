#include "Window.h"
#include <iostream>

Window::WindowClass Window::WindowClass::wndClass; // Define the static instance of the WindowClass

Window::WindowClass::WindowClass() noexcept
    : hInst(GetModuleHandle(nullptr))
{
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(wc);          // Set the size of the structure
    wc.style = CS_OWNDC;             // Class style
    wc.lpfnWndProc = HandleMsgSetup; // Window procedure
    wc.cbClsExtra = 0;               // Extra bytes after the window class
    wc.cbWndExtra = 0;               // Extra bytes after the window instance
    wc.hInstance = GetInstance();    // Handle to the instance
    wc.hIcon = nullptr;              // Handle to the class icon
    wc.hCursor = nullptr;            // Handle to the class cursor
    wc.hbrBackground = nullptr;      // Handle to the class background brush
    wc.lpszMenuName = nullptr;       // Resource name of the class menu
    wc.lpszClassName = GetName();    // Name of the window class
    wc.hIconSm = nullptr;            // Handle to the small icon

    RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() noexcept // destructor to unregister the window class when the program exits
{
    UnregisterClass(wndClassName, GetInstance());
}

const char *Window::WindowClass::GetName() noexcept
{
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return wndClass.hInst;
}

// Window constructor to create a window with the specified width, height, and name
Window::Window(int width, int height, const char *name) noexcept
{
    // Calculate the size of the window rectangle based on the desired client area size
    RECT wr;
    wr.left = 0;
    wr.top = 0;
    wr.right = width;
    wr.bottom = height;
    AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE); // Adjust the window rectangle to account for the non-client area (title bar, borders, etc.)
    // Create the window with the adjusted size and specified name
    hWnd = CreateWindowEx( // Optional window styles
        0,
        WindowClass::GetName(),                                                    // Window class name
        name,                                                                      // Window title
        WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_SYSMENU, // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,      // Size and position
        nullptr,                                                                   // Parent window
        nullptr,                                                                   // Menu
        WindowClass::GetInstance(),                                                // Instance handle
        this                                                                       // Additional application data (pointer to the Window instance)
    );
    // Show the window
    ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window() // destructor to destroy the window when the object goes out of scope
{
    DestroyWindow(hWnd);
}

// static window procedure to set up the message handling for the window
LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if (msg == WM_NCCREATE) // Check if the message is WM_NCCREATE, which is sent when the window is being created
    {
        // Extract the pointer to the Window instance from the CREATESTRUCT structure
        const CREATESTRUCTW *const pCreate = reinterpret_cast<CREATESTRUCTW *>(lParam);
        Window *const pWnd = static_cast<Window *>(pCreate->lpCreateParams);
        // Store the pointer to the Window instance in the user data of the window
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // Set the window procedure to HandleMsgThunk for future messages
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
        // Forward the message to the HandleMsg member function of the Window instance
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }
    // If it's not WM_NCCREATE, call the default window procedure
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

// static window procedure to handle messages for the window after setup
LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // Retrieve the pointer to the Window instance from the user data of the window
    Window *const pWnd = reinterpret_cast<Window *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    // Forward the message to the HandleMsg member function of the Window instance
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

// member function to handle messages for the window
LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0); // End message loop
        return 0;           // Prevent DefWindowProc from destroying again

    default:
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam); // Default message handling
}
