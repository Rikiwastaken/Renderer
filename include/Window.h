#pragma once
#include <optional>
#include "CustomWindows.h"
#include "Exceptions.h"
#include "Keyboard.h"
#include "Mouse.h"

class Window
{

public:
    class Exception : public RikiException
    {
    public:
        Exception(int line, const char *file, HRESULT hr) noexcept; // constructor to create an exception with the specified line number, file name, and HRESULT error code
        const char *what() const noexcept override;                 // override the what() function from RikiException to provide a description of the exception
        virtual const char *GetType() const noexcept override;      // override the GetType() function from RikiException to return the type of the exception
        static std::string TranslateErrorCode(HRESULT hr) noexcept; // static function to translate an HRESULT error code into a human-readable string
        HRESULT GetErrorCode() const noexcept;                      // function to get the HRESULT error code associated with the exception
        std::string GetErrorString() const noexcept;                // function to get a string representation of the error associated with the HRESULT error code

    private:
        HRESULT hr;
    };

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
    Window(int width, int height, const char *name);      // constructor to create a window with the specified width, height, and name
    ~Window();                                            // destructor to destroy the window when the object goes out of scope
    Window(const Window &) = delete;                      // delete copy constructor to prevent copying of the window object
    Window &operator=(const Window &) = delete;           // delete copy assignment operator to prevent copying of the window object
    void SetTitle(const std::string &title);              // function to set the title of the window
    static std::optional<int> ProcessMessages() noexcept; // static function to process Windows messages for the window and return an optional exit code if the window is closed
private:
    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept; // static window procedure to set up the message handling for the window
    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept; // static window procedure to handle messages for the window after setup
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;                      // member function to handle messages for the window
private:
    HWND hWnd;  // handle to the window
    int width;  // width of the window
    int height; // height of the window
public:
    Keyboard keyboard; // instance of the keyboard class to handle keyboard input for the window
    Mouse mouse;       // instance of the mouse class to handle mouse input for the window
};

// error exception helper macro
#define CHWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
#define CHWND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError())