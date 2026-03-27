#pragma once
#include <queue>
#include <array>

class Keyboard
{
    friend class Window;

public:
    class Event
    {
    public:
        enum class Type
        {
            Press,
            Release,
            Invalid
        };

    private:
        Type type;
        unsigned char code;

    public:
        Event() : type(Type::Invalid), code(0u) {}
        Event(Type type, unsigned char code) : type(type), code(code) {}
        bool IsPress() const noexcept { return type == Type::Press; }
        bool IsRelease() const noexcept { return type == Type::Release; }
        bool IsValid() const noexcept { return type != Type::Invalid; }
        unsigned char GetCode() const noexcept { return code; }
    };

public:
    Keyboard() = default;
    Keyboard(const Keyboard &) = delete;
    Keyboard &operator=(const Keyboard &) = delete;

    // key events
    bool KeyIsPressed(unsigned char keycode) const noexcept; // Check if a key is currently pressed
    Event ReadKey() noexcept;                                // Read the next key event from the queue
    bool KeyIsEmpty() const noexcept;                        // Check if there are no key events in the queue
    void ClearKey() noexcept;                                // Clear all key events from the queue
    // char events to read character input (e.g., for text input)
    char ReadChar() noexcept;          // Read the next character event from the queue
    bool CharIsEmpty() const noexcept; // Check if there are no character events in the queue
    void ClearChar() noexcept;         // Clear all character events from the queue
    void Clear() noexcept;             // Clear all events from both queues
    // autorepeat control
    void EnableAutorepeat() noexcept;          // Enable autorepeat for keys
    void DisableAutorepeat() noexcept;         // Disable autorepeat for keys
    bool AutorepeatIsEnabled() const noexcept; // Check if autorepeat is enabled
private:
    void OnKeyPressed(unsigned char keycode) noexcept;  // Handle a key press event
    void OnKeyReleased(unsigned char keycode) noexcept; // Handle a key release event
    void OnChar(char character) noexcept;               // Handle a character input event
    void ClearState() noexcept;                         // Clear the state of all keys (e.g., after a window resize)
    template <typename T>
    static void TrimBuffer(std::queue<T> &buffer) noexcept; // Trim the event buffer to prevent overflow
private:
    static constexpr unsigned int nKeys = 256u;     // Total number of possible key codes
    static constexpr unsigned int bufferSize = 16u; // Maximum size of the event buffers
    bool autorepeatEnabled = false;                 // Flag to indicate if autorepeat is enabled
    std::array<bool, nKeys> keyStates{};            // Array to track the state of each key (pressed or not)
    std::queue<Event> keyBuffer;                    // Queue to store key events
    std::queue<char> charBuffer;                    // Queue to store character events
};
