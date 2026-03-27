#pragma once
#include <queue>

class Mouse
{
    friend class Window;

public:
    class Event
    {
    public:
        enum class Type
        {
            LPress,
            LRelease,
            RPress,
            RRelease,
            WheelUp,
            WheelDown,
            Move,
            Invalid
        };

    private:
        Type type;
        unsigned char code; // Additional data for the event (e.g., mouse button code)
    public:
        Event() : type(Type::Invalid), code(0) {}
        Event(Type type, const Mouse &mouse) : type(type), code(0) {} // Constructor to create a mouse event with the specified type and associated mouse state
        bool IsLeftPress() const noexcept { return type == Type::LPress; }
        bool IsLeftRelease() const noexcept { return type == Type::LRelease; }
        bool IsRightPress() const noexcept { return type == Type::RPress; }
        bool IsRightRelease() const noexcept { return type == Type::RRelease; }
        bool IsWheelUp() const noexcept { return type == Type::WheelUp; }
        bool IsWheelDown() const noexcept { return type == Type::WheelDown; }
        bool IsMove() const noexcept { return type == Type::Move; }
        bool IsValid() const noexcept { return type != Type::Invalid; }
        unsigned char GetCode() const noexcept { return code; } // Get the additional data associated with the event (e.g., mouse button code)
    };

public:
    Mouse() = default;
    Mouse(const Mouse &) = delete;
    Mouse &operator=(const Mouse &) = delete;
    std::pair<int, int> GetPos() const noexcept; // Get the current position of the mouse cursor
    int GetPosX() const noexcept;                // Get the current X coordinate of the mouse
    int GetPosY() const noexcept;                // Get the current Y coordinate of the mouse
    bool LeftIsPressed() const noexcept;         // Check if the left mouse button is currently pressed
    bool RightIsPressed() const noexcept;        // Check if the right mouse button is currently pressed
    Mouse::Event Read() noexcept;                // Read the next mouse event from the queue
    bool IsEmpty() const noexcept                // Check if there are no mouse events in the queue
    {
        return mouseBuffer.empty();
    };
    void Clear() noexcept; // Clear all mouse events from the queue
private:
    void OnMouseMove(int x, int y) noexcept;     // Handle a mouse move event
    void OnLeftPressed(int x, int y) noexcept;   // Handle a left mouse button press event
    void OnLeftReleased(int x, int y) noexcept;  // Handle a left mouse button release event
    void OnRightPressed(int x, int y) noexcept;  // Handle a right mouse button press event
    void OnRightReleased(int x, int y) noexcept; // Handle a right mouse button release event
    void OnWheelUp(int x, int y) noexcept;       // Handle a mouse wheel up event
    void OnWheelDown(int x, int y) noexcept;     // Handle a mouse wheel
    void TrimBuffer() noexcept;                  // Trim the mouse event buffer to prevent overflow
private:
    static constexpr unsigned int bufferSize = 16u; // Maximum size of the mouse event buffer
    int x;                                          // current X coordinate of the mouse
    int y;                                          // current Y coordinate of the mouse
    bool leftIsPressed;                             // whether the left mouse button is currently pressed
    bool rightIsPressed;                            // whether the right mouse button is currently pressed
    std::queue<Event> mouseBuffer;                  // queue to store mouse events
};