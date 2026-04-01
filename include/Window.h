#pragma once
#include <optional>
#include "CustomWindows.h"
#include "Exceptions.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <memory>
#include "WindowsThrowMacros.h"

class Window
{

public:
    class Exception : public RikiException
    {
        using RikiException::RikiException;

    public:
        static std::string TranslateErrorCode(HRESULT hr) noexcept;
    };
    class HrException : public Exception
    {
    public:
        HrException(int line, const char *file, HRESULT hr) noexcept;
        const char *what() const noexcept override;
        const char *GetType() const noexcept override;
        HRESULT GetErrorCode() const noexcept;
        std::string GetErrorDescription() const noexcept;

    private:
        HRESULT hr;
    };
    class NoGfxException : public Exception
    {
    public:
        using Exception::Exception;
        const char *GetType() const noexcept override;
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
    Graphics &GetGraphics();                              // function to get a reference to the Graphics object associated with the window for rendering
private:
    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept; // static window procedure to set up the message handling for the window
    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept; // static window procedure to handle messages for the window after setup
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;                      // member function to handle messages for the window
private:
    HWND hWnd;                           // handle to the window
    int width;                           // width of the window
    int height;                          // height of the window
    std::unique_ptr<Graphics> pGraphics; // unique pointer to a Graphics object for rendering in the window
public:
    Keyboard keyboard; // instance of the keyboard class to handle keyboard input for the window
    Mouse mouse;       // instance of the mouse class to handle mouse input for the window
};