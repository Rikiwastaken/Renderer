#include "Mouse.h"
#include "Window.h"

std::pair<int, int> Mouse::GetPos() const noexcept // Get the current position of the mouse cursor
{
    return {x, y};
}

int Mouse::GetPosX() const noexcept // Get the current X coordinate of the mouse
{
    return x;
}

int Mouse::GetPosY() const noexcept // Get the current Y coordinate of the mouse
{
    return y;
}

bool Mouse::IsInWindow() const noexcept // Check if the mouse cursor is currently within the window
{
    return isInWindow;
}

bool Mouse::LeftIsPressed() const noexcept // Check if the left mouse button is currently pressed
{
    return leftIsPressed;
}

bool Mouse::RightIsPressed() const noexcept // Check if the right mouse button is currently pressed
{
    return rightIsPressed;
}

Mouse::Event Mouse::Read() noexcept // Read the next mouse event from the queue
{
    if (mouseBuffer.size() > 0u)
    {
        Mouse::Event e = mouseBuffer.front();
        mouseBuffer.pop();
        return e;
    }
    else
    {
        return Mouse::Event();
    }
}

void Mouse::Clear() noexcept // Clear all mouse events from the queue
{
    mouseBuffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int x, int y) noexcept // Handle a mouse move event
{
    this->x = x;
    this->y = y;
    mouseBuffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
    TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept // Handle a mouse enter event (when the cursor enters the window)
{
    isInWindow = true;
    mouseBuffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
    TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept // Handle a mouse leave event (when the cursor leaves the window)
{
    isInWindow = false;
    mouseBuffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
    TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y) noexcept // Handle a left mouse button press event
{
    leftIsPressed = true;
    mouseBuffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
    TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept // Handle a left mouse button release event
{
    leftIsPressed = false;
    mouseBuffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
    TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept // Handle a right mouse button press event
{
    rightIsPressed = true;
    mouseBuffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
    TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept // Handle a right mouse button release event
{
    rightIsPressed = false;
    mouseBuffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
    TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept // Handle a mouse wheel up event
{
    mouseBuffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
    TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept // Handle a mouse wheel down event
{
    mouseBuffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
    TrimBuffer();
}

void Mouse::TrimBuffer() noexcept // Trim the mouse event buffer to prevent overflow
{
    while (mouseBuffer.size() > bufferSize)
    {
        mouseBuffer.pop();
    }
}

void Mouse::OnWheelDelta(int x, int y, int delta) noexcept // Handle a mouse wheel event with the specified delta (positive for wheel up, negative for wheel down)
{
    wheelDeltaCarry += delta;
    while (wheelDeltaCarry >= WHEEL_DELTA)
    {
        wheelDeltaCarry -= WHEEL_DELTA;
        OnWheelUp(x, y);
    }
    while (wheelDeltaCarry <= -WHEEL_DELTA)
    {
        wheelDeltaCarry += WHEEL_DELTA;
        OnWheelDown(x, y);
    }
}