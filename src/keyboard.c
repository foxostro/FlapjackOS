#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <inout.h>
#include <kprintf.h>
#include <timer.h>
#include <panic.h>
#include <keyboard.h>

#define KEYBOARD_DATA_PORT     (0x60)
#define KEYBOARD_CONTROL_PORT  (0x64)

static const uint64_t g_make_codes[] = {
    0x1E,
    0x30,
    0x2E,
    0x20,
    0x12,
    0x21,
    0x22,
    0x23,
    0x17,
    0x24,
    0x25,
    0x26,
    0x32,
    0x31,
    0x18,
    0x19,
    0x10,
    0x13,
    0x1F,
    0x14,
    0x16,
    0x2F,
    0x11,
    0x2D,
    0x15,
    0x2C,
    0x0B,
    0x02,
    0x03,
    0x04,
    0x05,
    0x06,
    0x07,
    0x08,
    0x09,
    0x0A,
    0x29,
    0x0C,
    0x0D,
    0x2B,
    0x0E,
    0x39,
    0x0F,
    0x3A,
    0x2A,
    0x1D,
    0xE05B,
    0x38,
    0x36,
    0xE01D,
    0xE05C,
    0xE038,
    0xE05D,
    0x1C,
    0x01,
    0x3B,
    0x3C,
    0x3D,
    0x3E,
    0x3F,
    0x40,
    0x41,
    0x42,
    0x43,
    0x44,
    0x57,
    0x58,
    0xE02AE037,
    0x46,
    0xE11D45E19DC5,
    0x1A,
    0xE052,
    0xE047,
    0xE049,
    0xE053,
    0xE04F,
    0xE051,
    0xE048,
    0xE04B,
    0xE050,
    0xE04D,
    0x45,
    0xE035,
    0x37,
    0x4A,
    0x4E,
    0xE01C,
    0x53,
    0x52,
    0x4F,
    0x50,
    0x51,
    0x4B,
    0x4C,
    0x4D,
    0x47,
    0x48,
    0x49,
    0x1B,
    0x27,
    0x28,
    0x33,
    0x34,
    0x35,
    0xE05E,
    0xE05F,
    0xE063,
    0xE019,
    0xE010,
    0xE024,
    0xE022,
    0xE020,
    0xE030,
    0xE02E,
    0xE06D,
    0xE06C,
    0xE021,
    0xE06B,
    0xE065,
    0xE032,
    0xE06A,
    0xE069,
    0xE068,
    0xE067,
    0xE066
};

static const uint64_t g_break_codes[] = {
    0x9E,
    0xB0,
    0xAE,
    0xA0,
    0x92,
    0xA1,
    0xA2,
    0xA3,
    0x97,
    0xA4,
    0xA5,
    0xA6,
    0xB2,
    0xB1,
    0x98,
    0x99,
    0x90,
    0x93,
    0x9F,
    0x94,
    0x96,
    0xAF,
    0x91,
    0xAD,
    0x95,
    0xAC,
    0x8B,
    0x82,
    0x83,
    0x84,
    0x85,
    0x86,
    0x87,
    0x88,
    0x89,
    0x8A,
    0x89,
    0x8C,
    0x8D,
    0xAB,
    0x8E,
    0xB9,
    0x8F,
    0xBA,
    0xAA,
    0x9D,
    0xE0DB,
    0xB8,
    0xB6,
    0xE09D,
    0xE0DC,
    0xE0B8,
    0xE0DD,
    0x9C,
    0x81,
    0xBB,
    0xBC,
    0xBD,
    0xBE,
    0xBF,
    0xC0,
    0xC1,
    0xC2,
    0xC3,
    0xC4,
    0xD7,
    0xD8,
    0xE0B7E0AA,
    0xC6,
    0x00,
    0x9A,
    0xE0D2,
    0xE097,
    0xE0C9,
    0xE0D3,
    0xE0CF,
    0xE0D1,
    0xE0C8,
    0xE0CB,
    0xE0D0,
    0xE0CD,
    0xC5,
    0xE0B5,
    0xB7,
    0xCA,
    0xCE,
    0xE09C,
    0xD3,
    0xD2,
    0xCF,
    0xD0,
    0xD1,
    0xCB,
    0xCC,
    0xCD,
    0xC7,
    0xC8,
    0xC9,
    0x9B,
    0xA7,
    0xA8,
    0xB3,
    0xB4,
    0xB5,
    0xE0DE,
    0xE0DF,
    0xE0E3,
    0xE099,
    0xE090,
    0xE0A4,
    0xE0A2,
    0xE0A0,
    0xE0B0,
    0xE0AE,
    0xE0ED,
    0xE0EC,
    0xE0A1,
    0xE0EB,
    0xE0E5,
    0xE0B2,
    0xE0EA,
    0xE0E9,
    0xE0E8,
    0xE0E7,
    0xE0E6
};

static keycode_t find_key_code(const uint64_t *codes, size_t count, uint64_t scancode_sequence)
{
    for (size_t i = 0; i < count; ++i) {
        if (scancode_sequence == codes[i]) {
            return (keycode_t)i;
        }
    }
    return KEYCODE_MAX;
}

const char* keyboard_keycode_name(keycode_t key)
{
    switch (key) {
        case KEYCODE_A: return "KEYCODE_A";
        case KEYCODE_B: return "KEYCODE_B";
        case KEYCODE_C: return "KEYCODE_C";
        case KEYCODE_D: return "KEYCODE_D";
        case KEYCODE_E: return "KEYCODE_E";
        case KEYCODE_F: return "KEYCODE_F";
        case KEYCODE_G: return "KEYCODE_G";
        case KEYCODE_H: return "KEYCODE_H";
        case KEYCODE_I: return "KEYCODE_I";
        case KEYCODE_J: return "KEYCODE_J";
        case KEYCODE_K: return "KEYCODE_K";
        case KEYCODE_L: return "KEYCODE_L";
        case KEYCODE_M: return "KEYCODE_M";
        case KEYCODE_N: return "KEYCODE_N";
        case KEYCODE_O: return "KEYCODE_O";
        case KEYCODE_P: return "KEYCODE_P";
        case KEYCODE_Q: return "KEYCODE_Q";
        case KEYCODE_R: return "KEYCODE_R";
        case KEYCODE_S: return "KEYCODE_S";
        case KEYCODE_T: return "KEYCODE_T";
        case KEYCODE_U: return "KEYCODE_U";
        case KEYCODE_V: return "KEYCODE_V";
        case KEYCODE_W: return "KEYCODE_W";
        case KEYCODE_X: return "KEYCODE_X";
        case KEYCODE_Y: return "KEYCODE_Y";
        case KEYCODE_Z: return "KEYCODE_Z";
        case KEYCODE_0: return "KEYCODE_0";
        case KEYCODE_1: return "KEYCODE_1";
        case KEYCODE_2: return "KEYCODE_2";
        case KEYCODE_3: return "KEYCODE_3";
        case KEYCODE_4: return "KEYCODE_4";
        case KEYCODE_5: return "KEYCODE_5";
        case KEYCODE_6: return "KEYCODE_6";
        case KEYCODE_7: return "KEYCODE_7";
        case KEYCODE_8: return "KEYCODE_8";
        case KEYCODE_9: return "KEYCODE_9";
        case KEYCODE_GRAVE: return "KEYCODE_GRAVE";
        case KEYCODE_MINUS: return "KEYCODE_MINUS";
        case KEYCODE_EQUALS: return "KEYCODE_EQUALS";
        case KEYCODE_BACKSLASH: return "KEYCODE_BACKSLASH";
        case KEYCODE_BACKSPACE: return "KEYCODE_BACKSPACE";
        case KEYCODE_SPACE: return "KEYCODE_SPACE";
        case KEYCODE_TAB: return "KEYCODE_TAB";
        case KEYCODE_CAPSLOCK: return "KEYCODE_CAPSLOCK";
        case KEYCODE_LEFT_SHIFT: return "KEYCODE_LEFT_SHIFT";
        case KEYCODE_LEFT_CONTROL: return "KEYCODE_LEFT_CONTROL";
        case KEYCODE_LEFT_GUI: return "KEYCODE_LEFT_GUI";
        case KEYCODE_LEFT_ALT: return "KEYCODE_LEFT_ALT";
        case KEYCODE_RIGHT_SHIFT: return "KEYCODE_RIGHT_SHIFT";
        case KEYCODE_RIGHT_CONTROL: return "KEYCODE_RIGHT_CONTROL";
        case KEYCODE_RIGHT_GUI: return "KEYCODE_RIGHT_GUI";
        case KEYCODE_RIGHT_ALT: return "KEYCODE_RIGHT_ALT";
        case KEYCODE_APPS: return "KEYCODE_APPS";
        case KEYCODE_ENTER: return "KEYCODE_ENTER";
        case KEYCODE_ESCAPE: return "KEYCODE_ESCAPE";
        case KEYCODE_F1: return "KEYCODE_F1";
        case KEYCODE_F2: return "KEYCODE_F2";
        case KEYCODE_F3: return "KEYCODE_F3";
        case KEYCODE_F4: return "KEYCODE_F4";
        case KEYCODE_F5: return "KEYCODE_F5";
        case KEYCODE_F6: return "KEYCODE_F6";
        case KEYCODE_F7: return "KEYCODE_F7";
        case KEYCODE_F8: return "KEYCODE_F8";
        case KEYCODE_F9: return "KEYCODE_F9";
        case KEYCODE_F10: return "KEYCODE_F10";
        case KEYCODE_F11: return "KEYCODE_F11";
        case KEYCODE_F12: return "KEYCODE_F12";
        case KEYCODE_PRINT_SCREEN: return "KEYCODE_PRINT_SCREEN";
        case KEYCODE_SCROLL: return "KEYCODE_SCROLL";
        case KEYCODE_PAUSE: return "KEYCODE_PAUSE";
        case KEYCODE_OPEN_BRACKET: return "KEYCODE_OPEN_BRACKET";
        case KEYCODE_INSERT: return "KEYCODE_INSERT";
        case KEYCODE_HOME: return "KEYCODE_HOME";
        case KEYCODE_PAGE_UP: return "KEYCODE_PAGE_UP";
        case KEYCODE_DELETE: return "KEYCODE_DELETE";
        case KEYCODE_END: return "KEYCODE_END";
        case KEYCODE_PAGE_DOWN: return "KEYCODE_PAGE_DOWN";
        case KEYCODE_UP_ARROW: return "KEYCODE_UP_ARROW";
        case KEYCODE_LEFT_ARROW: return "KEYCODE_LEFT_ARROW";
        case KEYCODE_DOWN_ARROW: return "KEYCODE_DOWN_ARROW";
        case KEYCODE_RIGHT_ARROW: return "KEYCODE_RIGHT_ARROW";
        case KEYCODE_NUM: return "KEYCODE_NUM";
        case KEYCODE_NUMPAD_SLASH: return "KEYCODE_NUMPAD_SLASH";
        case KEYCODE_NUMPAD_ASTERISK: return "KEYCODE_NUMPAD_ASTERISK";
        case KEYCODE_NUMPAD_MINUS: return "KEYCODE_NUMPAD_MINUS";
        case KEYCODE_NUMPAD_PLUS: return "KEYCODE_NUMPAD_PLUS";
        case KEYCODE_NUMPAD_ENTER: return "KEYCODE_NUMPAD_ENTER";
        case KEYCODE_NUMPAD_PERIOD: return "KEYCODE_NUMPAD_PERIOD";
        case KEYCODE_NUMPAD_0: return "KEYCODE_NUMPAD_0";
        case KEYCODE_NUMPAD_1: return "KEYCODE_NUMPAD_1";
        case KEYCODE_NUMPAD_2: return "KEYCODE_NUMPAD_2";
        case KEYCODE_NUMPAD_3: return "KEYCODE_NUMPAD_3";
        case KEYCODE_NUMPAD_4: return "KEYCODE_NUMPAD_4";
        case KEYCODE_NUMPAD_5: return "KEYCODE_NUMPAD_5";
        case KEYCODE_NUMPAD_6: return "KEYCODE_NUMPAD_6";
        case KEYCODE_NUMPAD_7: return "KEYCODE_NUMPAD_7";
        case KEYCODE_NUMPAD_8: return "KEYCODE_NUMPAD_8";
        case KEYCODE_NUMPAD_9: return "KEYCODE_NUMPAD_9";
        case KEYCODE_CLOSE_BRACKET: return "KEYCODE_CLOSE_BRACKET";
        case KEYCODE_SEMICOLON: return "KEYCODE_SEMICOLON";
        case KEYCODE_QUOTE: return "KEYCODE_QUOTE";
        case KEYCODE_COMMA: return "KEYCODE_COMMA";
        case KEYCODE_PERIOD: return "KEYCODE_PERIOD";
        case KEYCODE_SLASH: return "KEYCODE_SLASH";
        case KEYCODE_POWER: return "KEYCODE_POWER";
        case KEYCODE_SLEEP: return "KEYCODE_SLEEP";
        case KEYCODE_WAKE: return "KEYCODE_WAKE";
        case KEYCODE_NEXT_TRACK: return "KEYCODE_NEXT_TRACK";
        case KEYCODE_PREV_TRACK: return "KEYCODE_PREV_TRACK";
        case KEYCODE_STOP: return "KEYCODE_STOP";
        case KEYCODE_PLAY_PAUSE: return "KEYCODE_PLAY_PAUSE";
        case KEYCODE_MUTE: return "KEYCODE_MUTE";
        case KEYCODE_VOLUME_UP: return "KEYCODE_VOLUME_UP";
        case KEYCODE_VOLUME_DOWN: return "KEYCODE_VOLUME_DOWN";
        case KEYCODE_MEDIA_SELECT: return "KEYCODE_MEDIA_SELECT";
        case KEYCODE_EMAIL: return "KEYCODE_EMAIL";
        case KEYCODE_CALCULATOR: return "KEYCODE_CALCULATOR";
        case KEYCODE_MY_COMPUTER: return "KEYCODE_MY_COMPUTER";
        case KEYCODE_WWW_SEARCH: return "KEYCODE_WWW_SEARCH";
        case KEYCODE_WWW_HOME: return "KEYCODE_WWW_HOME";
        case KEYCODE_WWW_BACK: return "KEYCODE_WWW_BACK";
        case KEYCODE_WWW_FORWARD: return "KEYCODE_WWW_FORWARD";
        case KEYCODE_WWW_STOP: return "KEYCODE_WWW_STOP";
        case KEYCODE_WWW_REFRESH: return "KEYCODE_WWW_REFRESH";
        case KEYCODE_WWW_FAVORITES: return "KEYCODE_WWW_FAVORITES";
        default: return "UNKNOWN";
    }
}

void keyboard_int_handler()
{
    uint8_t scancode = 0, prev_scancode = 0;
    uint64_t scancode_sequence = 0;

    kprintf("keyboard interrupt -- ");

    while (true) {
        scancode = inb(KEYBOARD_DATA_PORT);

        if (prev_scancode == scancode) {
            break;
        } else {
            kprintf("%x ", scancode);
            scancode_sequence <<= 8;
            scancode_sequence |= scancode;
        }

        prev_scancode = scancode;
    }

    kprintf(" scancode=0x%x", scancode);

    bool key_pressed = true;
    keycode_t key = find_key_code(g_make_codes, KEYCODE_MAX, scancode_sequence);

    if (key == KEYCODE_MAX) {
        key_pressed = false;
        key = find_key_code(g_break_codes, KEYCODE_MAX, scancode_sequence);
    }

    kprintf(" \"%s\" (0x%x) was %s ; time=%u\n",
            keyboard_keycode_name(key), key,
            key_pressed ? "pressed" : "released",
            timer_seconds());
}

void keyboard_init()
{
    // Nothing to do.
}
