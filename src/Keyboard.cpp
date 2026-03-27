#include "Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept // Check if a key is currently pressed
{
    return keyStates[keycode];
}

Keyboard::Event Keyboard::ReadKey() noexcept // Read the next key event from the queue
{
    if (keyBuffer.size() > 0u)
    {
        Keyboard::Event e = keyBuffer.front();
        keyBuffer.pop();
        return e;
    }
    else
    {
        return Keyboard::Event();
    }
}

bool Keyboard::KeyIsEmpty() const noexcept // Check if there are no key events in the queue
{
    return keyBuffer.empty();
}

void Keyboard::ClearKey() noexcept // Clear all key events from the queue
{
    keyBuffer = std::queue<Event>();
}

char Keyboard::ReadChar() noexcept // Read the next character event from the queue
{
    if (charBuffer.size() > 0u)
    {
        char charcode = charBuffer.front();
        charBuffer.pop();
        return charcode;
    }
    else
    {
        return 0;
    }
}

bool Keyboard::CharIsEmpty() const noexcept // Check if there are no character events in the queue
{
    return charBuffer.empty();
}

void Keyboard::ClearChar() noexcept // Clear all character events from the queue
{
    charBuffer = std::queue<char>();
}

void Keyboard::Clear() noexcept // Clear all events from both queues
{
    ClearKey();
    ClearChar();
}

void Keyboard::EnableAutorepeat() noexcept // Enable autorepeat for keys
{
    autorepeatEnabled = true;
}

void Keyboard::DisableAutorepeat() noexcept // Disable autorepeat for keys
{
    autorepeatEnabled = false;
}

bool Keyboard::AutorepeatIsEnabled() const noexcept // Check if autorepeat is enabled
{
    return autorepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept // Handle a key press event
{
    keyStates[keycode] = true;
    keyBuffer.push(Event(Event::Type::Press, keycode));
    TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept // Handle a key release event
{
    keyStates[keycode] = false;
    keyBuffer.push(Event(Event::Type::Release, keycode));
    TrimBuffer(keyBuffer);
}

void Keyboard::OnChar(char character) noexcept // Handle a character input event
{
    charBuffer.push(character);
    TrimBuffer(charBuffer);
}

void Keyboard::ClearState() noexcept // Clear the state of all keys (e.g., after a window resize)
{
    keyStates.fill(false);
}

template <typename T>
void Keyboard::TrimBuffer(std::queue<T> &buffer) noexcept
{
    while (buffer.size() > bufferSize)
    {
        buffer.pop();
    }
}
