#include "Window.h"
#include <iostream>
#include <sstream>
#include "Exceptions.h"
#include "Resource.h"

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
    wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR));
    // Handle to the class icon
    wc.hCursor = nullptr;                                                                                               // Handle to the class cursor
    wc.hbrBackground = nullptr;                                                                                         // Handle to the class background brush
    wc.lpszMenuName = nullptr;                                                                                          // Resource name of the class menu
    wc.lpszClassName = GetName();                                                                                       // Name of the window class
    wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR)); // Handle to the small icon

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
Window::Window(int width, int height, const char *name) : width(width), height(height)
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
    HICON hIcon = static_cast<HICON>(
        LoadImage(WindowClass::GetInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR));
    HICON hIconSm = static_cast<HICON>(
        LoadImage(WindowClass::GetInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR));

    if (!hIcon)
        hIcon = hIconSm;

    if (!hIconSm)
        hIconSm = hIcon;

    SendMessage(hWnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon));
    SendMessage(hWnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIconSm));
    // Show the window
    ShowWindow(hWnd, SW_SHOW);
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
    case WM_CREATE:
    {
        // Load icons
        HICON hIcon = static_cast<HICON>(LoadImage(WindowClass::GetInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR));
        HICON hIconSm = static_cast<HICON>(LoadImage(WindowClass::GetInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR));

        if (!hIcon)
            hIcon = hIconSm;
        if (!hIconSm)
            hIconSm = hIcon;

        SendMessage(hWnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon));
        SendMessage(hWnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIconSm));
        break;
    }
    case WM_CLOSE:
        PostQuitMessage(0); // End message loop
        return 0;           // Prevent DefWindowProc from destroying again
    // Handle keyboard input
    case WM_KEYDOWN:
    {
        if (!(lParam & 0x40000000) || keyboard.AutorepeatIsEnabled()) // Check if the key is being held down (autorepeat) and if autorepeat is enabled
        {
            keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
        }
        break;
    }
    case WM_SYSKEYDOWN:
    {
        if (!(lParam & 0x40000000) || keyboard.AutorepeatIsEnabled()) // Check if the key is being held down (autorepeat) and if autorepeat is enabled
        {
            keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
        }
        break;
    }
    case WM_KEYUP:
    {
        keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
        break;
    }
    case WM_SYSKEYUP:
    {
        keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
        break;
    }
    case WM_CHAR:
    {
        keyboard.OnChar(static_cast<char>(wParam));
        break;
    }

    // reset key states when the window is resized to prevent input issues
    case WM_SIZE:
    {
        keyboard.ClearState();
        break;
    }
    // reset key states when the window is destroyed to prevent input issues
    case WM_DESTROY:
    {
        keyboard.ClearState();
        break;
    }
    // reset key states when the window is loses focus to prevent input issues
    case WM_KILLFOCUS:
    {
        keyboard.ClearState();
        break;
    }

        // Handle mouse input
    case WM_MOUSEMOVE:
    {
        POINTS pt = MAKEPOINTS(lParam);
        if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
        {
            mouse.OnMouseMove(pt.x, pt.y);
            if (!mouse.IsInWindow())
            {
                SetCapture(hWnd); // Capture the mouse input to receive mouse events even when the cursor is outside the window
                mouse.OnMouseEnter();
            }
        }
        else
        {
            if (wParam & (MK_LBUTTON | MK_RBUTTON)) // If a mouse button is pressed, treat it as a move event to update the mouse position
            {
                mouse.OnMouseMove(pt.x, pt.y);
            }
            else
            {
                ReleaseCapture(); // Release the mouse capture when the cursor leaves the window and no buttons are pressed
                mouse.OnMouseLeave();
            }
        }
        break;
    }
    case WM_LBUTTONDOWN:
    {
        POINTS pt = MAKEPOINTS(lParam);
        mouse.OnLeftPressed(pt.x, pt.y);
        break;
    }
    case WM_LBUTTONUP:
    {
        POINTS pt = MAKEPOINTS(lParam);
        mouse.OnLeftReleased(pt.x, pt.y);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        POINTS pt = MAKEPOINTS(lParam);
        mouse.OnRightPressed(pt.x, pt.y);
        break;
    }
    case WM_RBUTTONUP:
    {
        POINTS pt = MAKEPOINTS(lParam);
        mouse.OnRightReleased(pt.x, pt.y);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        mouse.OnWheelDelta(pt.x, pt.y, delta);
        break;
    }

    default:
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam); // Default message handling
}

// Window Exception
Window::Exception::Exception(int line, const char *file, HRESULT hr) noexcept
    : RikiException(line, file), hr(hr)
{
}

const char *Window::Exception::what() const noexcept
{
    std::ostringstream oss; // Create a string stream to build the what() message
    oss << GetType() << std::endl
        << GetOriginString() << std::endl
        << "[Error Code] " << GetErrorCode() << std::endl
        << "[Description] " << GetErrorString(); // Append the type of the exception, origin string, error code, and error description to the string stream
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char *Window::Exception::GetType() const noexcept
{
    return "Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
    char *pMsgBuf = nullptr;
    DWORD nMsgLen = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, // Flags to specify how the message should be formatted
        nullptr,                                                                                     // Source of the message (nullptr for system messages)
        hr,                                                                                          // The error code to translate
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),                                                   // Language for the message
        reinterpret_cast<char *>(&pMsgBuf),                                                          // Buffer to receive the formatted message
        0,                                                                                           // Size of the buffer (0 for automatic allocation)
        nullptr                                                                                      // Arguments for the message (not used here)
    );
    if (nMsgLen == 0) // If FormatMessage fails, return a default error message
    {
        return "Unidentified error code";
    }
    std::string errorString(pMsgBuf); // Convert the formatted message to a std::string
    LocalFree(pMsgBuf);               // Free the buffer allocated by FormatMessage
    return errorString;               // Return the translated error string
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
    return hr; // Return the HRESULT error code associated with the exception
}

std::string Window::Exception::GetErrorString() const noexcept
{
    return TranslateErrorCode(hr); // Return a string representation of the error associated with the HRESULT error code
}