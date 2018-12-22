#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_KEYBOARD_DEVICE_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_KEYBOARD_DEVICE_HPP

// Abstract interface for access to the keyboard.

enum {
    KEYCODE_A = 0,
    KEYCODE_B,
    KEYCODE_C,
    KEYCODE_D,
    KEYCODE_E,
    KEYCODE_F,
    KEYCODE_G,
    KEYCODE_H,
    KEYCODE_I,
    KEYCODE_J,
    KEYCODE_K,
    KEYCODE_L,
    KEYCODE_M,
    KEYCODE_N,
    KEYCODE_O,
    KEYCODE_P,
    KEYCODE_Q,
    KEYCODE_R,
    KEYCODE_S,
    KEYCODE_T,
    KEYCODE_U,
    KEYCODE_V,
    KEYCODE_W,
    KEYCODE_X,
    KEYCODE_Y,
    KEYCODE_Z,
    KEYCODE_0,
    KEYCODE_1,
    KEYCODE_2,
    KEYCODE_3,
    KEYCODE_4,
    KEYCODE_5,
    KEYCODE_6,
    KEYCODE_7,
    KEYCODE_8,
    KEYCODE_9,
    KEYCODE_GRAVE,
    KEYCODE_MINUS,
    KEYCODE_EQUALS,
    KEYCODE_BACKSLASH,
    KEYCODE_BACKSPACE,
    KEYCODE_SPACE,
    KEYCODE_TAB,
    KEYCODE_CAPSLOCK,
    KEYCODE_LEFT_SHIFT,
    KEYCODE_LEFT_CONTROL,
    KEYCODE_LEFT_GUI,
    KEYCODE_LEFT_ALT,
    KEYCODE_RIGHT_SHIFT,
    KEYCODE_RIGHT_CONTROL,
    KEYCODE_RIGHT_GUI,
    KEYCODE_RIGHT_ALT,
    KEYCODE_APPS,
    KEYCODE_ENTER,
    KEYCODE_ESCAPE,
    KEYCODE_F1,
    KEYCODE_F2,
    KEYCODE_F3,
    KEYCODE_F4,
    KEYCODE_F5,
    KEYCODE_F6,
    KEYCODE_F7,
    KEYCODE_F8,
    KEYCODE_F9,
    KEYCODE_F10,
    KEYCODE_F11,
    KEYCODE_F12,
    KEYCODE_PRINT_SCREEN,
    KEYCODE_SCROLL,
    KEYCODE_PAUSE,
    KEYCODE_OPEN_BRACKET,
    KEYCODE_INSERT,
    KEYCODE_HOME,
    KEYCODE_PAGE_UP,
    KEYCODE_DELETE,
    KEYCODE_END,
    KEYCODE_PAGE_DOWN,
    KEYCODE_UP_ARROW,
    KEYCODE_LEFT_ARROW,
    KEYCODE_DOWN_ARROW,
    KEYCODE_RIGHT_ARROW,
    KEYCODE_NUM,
    KEYCODE_NUMPAD_SLASH,
    KEYCODE_NUMPAD_ASTERISK,
    KEYCODE_NUMPAD_MINUS,
    KEYCODE_NUMPAD_PLUS,
    KEYCODE_NUMPAD_ENTER,
    KEYCODE_NUMPAD_PERIOD,
    KEYCODE_NUMPAD_0,
    KEYCODE_NUMPAD_1,
    KEYCODE_NUMPAD_2,
    KEYCODE_NUMPAD_3,
    KEYCODE_NUMPAD_4,
    KEYCODE_NUMPAD_5,
    KEYCODE_NUMPAD_6,
    KEYCODE_NUMPAD_7,
    KEYCODE_NUMPAD_8,
    KEYCODE_NUMPAD_9,
    KEYCODE_CLOSE_BRACKET,
    KEYCODE_SEMICOLON,
    KEYCODE_QUOTE,
    KEYCODE_COMMA,
    KEYCODE_PERIOD,
    KEYCODE_SLASH,
    KEYCODE_POWER,
    KEYCODE_SLEEP,
    KEYCODE_WAKE,
    KEYCODE_NEXT_TRACK,
    KEYCODE_PREV_TRACK,
    KEYCODE_STOP,
    KEYCODE_PLAY_PAUSE,
    KEYCODE_MUTE,
    KEYCODE_VOLUME_UP,
    KEYCODE_VOLUME_DOWN,
    KEYCODE_MEDIA_SELECT,
    KEYCODE_EMAIL,
    KEYCODE_CALCULATOR,
    KEYCODE_MY_COMPUTER,
    KEYCODE_WWW_SEARCH,
    KEYCODE_WWW_HOME,
    KEYCODE_WWW_BACK,
    KEYCODE_WWW_FORWARD,
    KEYCODE_WWW_STOP,
    KEYCODE_WWW_REFRESH,
    KEYCODE_WWW_FAVORITES,
    KEYCODE_MAX
};

using Keycode = unsigned;

enum KeycodeKeyState {
    PRESSED, RELEASED
};

struct KeyboardEvent {
    // Identifies which button on the keyboard corresponds to this event.
    Keycode key;

    // The new state of the key with this event, e.g., PRESSED or RELEASED.
    KeycodeKeyState state;

    // The ASCII character associated with the key press, or zero if none.
    char ch;

    // Default constructor.
    KeyboardEvent()
     : key(KEYCODE_MAX),
       state(RELEASED),
       ch(0)
    {}

    // Constructor.
    KeyboardEvent(Keycode k, KeycodeKeyState s, char c)
     : key(k),
       state(s),
       ch(c)
    {}
};

class KeyboardDevice {
public:
    virtual ~KeyboardDevice() = default;

    // Returns the name of the given key code.
    virtual const char* keycode_name(Keycode key) = 0;

    // Gets the next key event.
    // Blocks on the next key event and returns it in the specified event structure.
    virtual KeyboardEvent get_event() = 0;
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_KEYBOARD_DEVICE_HPP
