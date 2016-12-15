#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <inout.h>
#include <console.h>
#include <ctypes.h>
#include <panic.h>
#include <keyboard.h>

#define KEYBOARD_DATA_PORT     (0x60)
#define KEYBOARD_CONTROL_PORT  (0x64)
#define MAX_SCANCODE_BYTES     (6)

typedef enum {
    IDLE,
    PROCESSING_ESCAPE_CODE
} keyboard_state_t;

typedef struct {
    bool pressed;
    bool shift;
    keycode_t key;
    char ch;
} keyboard_output_t;

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
        default: return "Unknown Key Code";
    }
}

static void keyboard_clear_input()
{
    for (int i = 0; i < MAX_SCANCODE_BYTES; ++i) {
        (void)inb(KEYBOARD_DATA_PORT);       
    }
}

static bool keyboard_step_idle(unsigned scancode, keyboard_state_t *state, keyboard_output_t *output)
{
    switch (scancode) {
        case 0x1E:
            output->key = KEYCODE_A;
            output->pressed = true;
            output->ch = output->shift ? 'A' : 'a';
            return false;

        case 0x30:
            output->key = KEYCODE_B;
            output->pressed = true;
            output->ch = output->shift ? 'B' : 'b';
            return false;

        case 0x2E:
            output->key = KEYCODE_C;
            output->pressed = true;
            output->ch = output->shift ? 'C' : 'c';
            return false;

        case 0x20:
            output->key = KEYCODE_D;
            output->pressed = true;
            output->ch = output->shift ? 'D' : 'd';
            return false;

        case 0x12:
            output->key = KEYCODE_E;
            output->pressed = true;
            output->ch = output->shift ? 'E' : 'e';
            return false;

        case 0x21:
            output->key = KEYCODE_F;
            output->pressed = true;
            output->ch = output->shift ? 'F' : 'f';
            return false;

        case 0x22:
            output->key = KEYCODE_G;
            output->pressed = true;
            output->ch = output->shift ? 'G' : 'g';
            return false;

        case 0x23:
            output->key = KEYCODE_H;
            output->pressed = true;
            output->ch = output->shift ? 'H' : 'h';
            return false;

        case 0x17:
            output->key = KEYCODE_I;
            output->pressed = true;
            output->ch = output->shift ? 'I' : 'i';
            return false;

        case 0x24:
            output->key = KEYCODE_J;
            output->pressed = true;
            output->ch = output->shift ? 'J' : 'j';
            return false;

        case 0x25:
            output->key = KEYCODE_K;
            output->pressed = true;
            output->ch = output->shift ? 'K' : 'k';
            return false;

        case 0x26:
            output->key = KEYCODE_L;
            output->pressed = true;
            output->ch = output->shift ? 'L' : 'l';
            return false;

        case 0x32:
            output->key = KEYCODE_M;
            output->pressed = true;
            output->ch = output->shift ? 'M' : 'm';
            return false;

        case 0x31:
            output->key = KEYCODE_N;
            output->pressed = true;
            output->ch = output->shift ? 'N' : 'n';
            return false;

        case 0x18:
            output->key = KEYCODE_O;
            output->pressed = true;
            output->ch = output->shift ? 'O' : 'o';
            return false;

        case 0x19:
            output->key = KEYCODE_P;
            output->pressed = true;
            output->ch = output->shift ? 'P' : 'p';
            return false;

        case 0x10:
            output->key = KEYCODE_Q;
            output->pressed = true;
            output->ch = output->shift ? 'Q' : 'q';
            return false;

        case 0x13:
            output->key = KEYCODE_R;
            output->pressed = true;
            output->ch = output->shift ? 'R' : 'r';
            return false;

        case 0x1F:
            output->key = KEYCODE_S;
            output->pressed = true;
            output->ch = output->shift ? 'S' : 's';
            return false;

        case 0x14:
            output->key = KEYCODE_T;
            output->pressed = true;
            output->ch = output->shift ? 'T' : 't';
            return false;

        case 0x16:
            output->key = KEYCODE_U;
            output->pressed = true;
            output->ch = output->shift ? 'U' : 'u';
            return false;

        case 0x2F:
            output->key = KEYCODE_V;
            output->pressed = true;
            output->ch = output->shift ? 'V' : 'v';
            return false;

        case 0x11:
            output->key = KEYCODE_W;
            output->pressed = true;
            output->ch = output->shift ? 'W' : 'w';
            return false;

        case 0x2D:
            output->key = KEYCODE_X;
            output->pressed = true;
            output->ch = output->shift ? 'X' : 'x';
            return false;

        case 0x15:
            output->key = KEYCODE_Y;
            output->pressed = true;
            output->ch = output->shift ? 'Y' : 'y';
            return false;

        case 0x2C:
            output->key = KEYCODE_Z;
            output->pressed = true;
            output->ch = output->shift ? 'Z' : 'z';
            return false;

        case 0x0B:
            output->key = KEYCODE_0;
            output->pressed = true;
            output->ch = output->shift ? ')' : '0';
            return false;

        case 0x2:
            output->key = KEYCODE_1;
            output->pressed = true;
            output->ch = output->shift ? '!' : '1';
            return false;

        case 0x3:
            output->key = KEYCODE_2;
            output->pressed = true;
            output->ch = output->shift ? '@' : '2';
            return false;

        case 0x4:
            output->key = KEYCODE_3;
            output->pressed = true;
            output->ch = output->shift ? '#' : '3';
            return false;

        case 0x5:
            output->key = KEYCODE_4;
            output->pressed = true;
            output->ch = output->shift ? '$' : '4';
            return false;

        case 0x6:
            output->key = KEYCODE_5;
            output->pressed = true;
            output->ch = output->shift ? '%' : '5';
            return false;

        case 0x7:
            output->key = KEYCODE_6;
            output->pressed = true;
            output->ch = output->shift ? '^' : '6';
            return false;

        case 0x8:
            output->key = KEYCODE_7;
            output->pressed = true;
            output->ch = output->shift ? '&' : '7';
            return false;

        case 0x9:
            output->key = KEYCODE_8;
            output->pressed = true;
            output->ch = output->shift ? '*' : '8';
            return false;

        case 0x0A:
            output->key = KEYCODE_9;
            output->pressed = true;
            output->ch = output->shift ? '(' : '9';
            return false;

        case 0x29:
            output->key = KEYCODE_GRAVE;
            output->pressed = true;
            output->ch = output->shift ? '~' : '`';
            return false;

        case 0x0C:
            output->key = KEYCODE_MINUS;
            output->pressed = true;
            output->ch = output->shift ? '_' : '-';
            return false;

        case 0x0D:
            output->key = KEYCODE_EQUALS;
            output->pressed = true;
            output->ch = output->shift ? '+' : '=';
            return false;

        case 0x2B:
            output->key = KEYCODE_BACKSLASH;
            output->pressed = true;
            output->ch = output->shift ? '|' : '\\';
            return false;

        case 0x0E:
            output->key = KEYCODE_BACKSPACE;
            output->pressed = true;
            output->ch = 8;
            return false;

        case 0x39:
            output->key = KEYCODE_SPACE;
            output->pressed = true;
            output->ch = ' ';
            return false;

        case 0x0F:
            output->key = KEYCODE_TAB;
            output->pressed = true;
            output->ch = '\t';
            return false;

        case 0x3A:
            output->key = KEYCODE_CAPSLOCK;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x2A:
            output->key = KEYCODE_LEFT_SHIFT;
            output->pressed = true;
            output->ch = 0;
            output->shift = true;
            return false;

        case 0x1D:
            output->key = KEYCODE_LEFT_CONTROL;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x38:
            output->key = KEYCODE_LEFT_ALT;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x36:
            output->key = KEYCODE_RIGHT_SHIFT;
            output->pressed = true;
            output->ch = 0;
            output->shift = true;
            return false;

        case 0x1C:
            output->key = KEYCODE_ENTER;
            output->pressed = true;
            output->ch = '\n';
            return false;

        case 0x1:
            output->key = KEYCODE_ESCAPE;
            output->pressed = true;
            output->ch = 27;
            return false;

        case 0x3B:
            output->key = KEYCODE_F1;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x3C:
            output->key = KEYCODE_F2;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x3D:
            output->key = KEYCODE_F3;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x3E:
            output->key = KEYCODE_F4;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x3F:
            output->key = KEYCODE_F5;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x40:
            output->key = KEYCODE_F6;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x41:
            output->key = KEYCODE_F7;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x42:
            output->key = KEYCODE_F8;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x43:
            output->key = KEYCODE_F9;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x44:
            output->key = KEYCODE_F10;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x57:
            output->key = KEYCODE_F11;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x58:
            output->key = KEYCODE_F12;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x46:
            output->key = KEYCODE_SCROLL;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x1A:
            output->key = KEYCODE_OPEN_BRACKET;
            output->pressed = true;
            output->ch = output->shift ? '{' : '[';
            return false;

        case 0x45:
            output->key = KEYCODE_NUM;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x37:
            output->key = KEYCODE_NUMPAD_ASTERISK;
            output->pressed = true;
            output->ch = '*';
            return false;

        case 0x4A:
            output->key = KEYCODE_NUMPAD_MINUS;
            output->pressed = true;
            output->ch = '-';
            return false;

        case 0x4E:
            output->key = KEYCODE_NUMPAD_PLUS;
            output->pressed = true;
            output->ch = '+';
            return false;

        case 0x53:
            output->key = KEYCODE_NUMPAD_PERIOD;
            output->pressed = true;
            output->ch = '.';
            return false;

        case 0x52:
            output->key = KEYCODE_NUMPAD_0;
            output->pressed = true;
            output->ch = '0';
            return false;

        case 0x4F:
            output->key = KEYCODE_NUMPAD_1;
            output->pressed = true;
            output->ch = '1';
            return false;

        case 0x50:
            output->key = KEYCODE_NUMPAD_2;
            output->pressed = true;
            output->ch = '2';
            return false;

        case 0x51:
            output->key = KEYCODE_NUMPAD_3;
            output->pressed = true;
            output->ch = '3';
            return false;

        case 0x4B:
            output->key = KEYCODE_NUMPAD_4;
            output->pressed = true;
            output->ch = '4';
            return false;

        case 0x4C:
            output->key = KEYCODE_NUMPAD_5;
            output->pressed = true;
            output->ch = '5';
            return false;

        case 0x4D:
            output->key = KEYCODE_NUMPAD_6;
            output->pressed = true;
            output->ch = '6';
            return false;

        case 0x47:
            output->key = KEYCODE_NUMPAD_7;
            output->pressed = true;
            output->ch = '7';
            return false;

        case 0x48:
            output->key = KEYCODE_NUMPAD_8;
            output->pressed = true;
            output->ch = '8';
            return false;

        case 0x49:
            output->key = KEYCODE_NUMPAD_9;
            output->pressed = true;
            output->ch = '9';
            return false;

        case 0x1B:
            output->key = KEYCODE_CLOSE_BRACKET;
            output->pressed = true;
            output->ch = output->shift ? '}' : ']';
            return false;

        case 0x27:
            output->key = KEYCODE_SEMICOLON;
            output->pressed = true;
            output->ch = output->shift ? ':' : ';';
            return false;

        case 0x28:
            output->key = KEYCODE_QUOTE;
            output->pressed = true;
            output->ch = output->shift ? '"' : '\'';
            return false;

        case 0x33:
            output->key = KEYCODE_COMMA;
            output->pressed = true;
            output->ch = output->shift ? '<' : ',';
            return false;

        case 0x34:
            output->key = KEYCODE_PERIOD;
            output->pressed = true;
            output->ch = output->shift ? '>' : '.';
            return false;

        case 0x35:
            output->key = KEYCODE_SLASH;
            output->pressed = true;
            output->ch = output->shift ? '?' : '/';
            return false;

        case 0x9E:
            output->key = KEYCODE_A;
            output->pressed = false;
            output->ch = output->shift ? 'A' : 'a';
            return false;

        case 0xB0:
            output->key = KEYCODE_B;
            output->pressed = false;
            output->ch = output->shift ? 'B' : 'b';
            return false;

        case 0xAE:
            output->key = KEYCODE_C;
            output->pressed = false;
            output->ch = output->shift ? 'C' : 'c';
            return false;

        case 0xA0:
            output->key = KEYCODE_D;
            output->pressed = false;
            output->ch = output->shift ? 'D' : 'd';
            return false;

        case 0x92:
            output->key = KEYCODE_E;
            output->pressed = false;
            output->ch = output->shift ? 'E' : 'e';
            return false;

        case 0xA1:
            output->key = KEYCODE_F;
            output->pressed = false;
            output->ch = output->shift ? 'F' : 'f';
            return false;

        case 0xA2:
            output->key = KEYCODE_G;
            output->pressed = false;
            output->ch = output->shift ? 'G' : 'g';
            return false;

        case 0xA3:
            output->key = KEYCODE_H;
            output->pressed = false;
            output->ch = output->shift ? 'H' : 'h';
            return false;

        case 0x97:
            output->key = KEYCODE_I;
            output->pressed = false;
            output->ch = output->shift ? 'I' : 'i';
            return false;

        case 0xA4:
            output->key = KEYCODE_J;
            output->pressed = false;
            output->ch = output->shift ? 'J' : 'j';
            return false;

        case 0xA5:
            output->key = KEYCODE_K;
            output->pressed = false;
            output->ch = output->shift ? 'K' : 'k';
            return false;

        case 0xA6:
            output->key = KEYCODE_L;
            output->pressed = false;
            output->ch = output->shift ? 'L' : 'l';
            return false;

        case 0xB2:
            output->key = KEYCODE_M;
            output->pressed = false;
            output->ch = output->shift ? 'M' : 'm';
            return false;

        case 0xB1:
            output->key = KEYCODE_N;
            output->pressed = false;
            output->ch = output->shift ? 'N' : 'n';
            return false;

        case 0x98:
            output->key = KEYCODE_O;
            output->pressed = false;
            output->ch = output->shift ? 'O' : 'o';
            return false;

        case 0x99:
            output->key = KEYCODE_P;
            output->pressed = false;
            output->ch = output->shift ? 'P' : 'p';
            return false;

        case 0x90:
            output->key = KEYCODE_Q;
            output->pressed = false;
            output->ch = output->shift ? 'Q' : 'q';
            return false;

        case 0x93:
            output->key = KEYCODE_R;
            output->pressed = false;
            output->ch = output->shift ? 'R' : 'r';
            return false;

        case 0x9F:
            output->key = KEYCODE_S;
            output->pressed = false;
            output->ch = output->shift ? 'S' : 's';
            return false;

        case 0x94:
            output->key = KEYCODE_T;
            output->pressed = false;
            output->ch = output->shift ? 'T' : 't';
            return false;

        case 0x96:
            output->key = KEYCODE_U;
            output->pressed = false;
            output->ch = output->shift ? 'U' : 'u';
            return false;

        case 0xAF:
            output->key = KEYCODE_V;
            output->pressed = false;
            output->ch = output->shift ? 'V' : 'v';
            return false;

        case 0x91:
            output->key = KEYCODE_W;
            output->pressed = false;
            output->ch = output->shift ? 'W' : 'w';
            return false;

        case 0xAD:
            output->key = KEYCODE_X;
            output->pressed = false;
            output->ch = output->shift ? 'X' : 'x';
            return false;

        case 0x95:
            output->key = KEYCODE_Y;
            output->pressed = false;
            output->ch = output->shift ? 'Y' : 'y';
            return false;

        case 0xAC:
            output->key = KEYCODE_Z;
            output->pressed = false;
            output->ch = output->shift ? 'Z' : 'z';
            return false;

        case 0x8B:
            output->key = KEYCODE_0;
            output->pressed = false;
            output->ch = '0';
            return false;

        case 0x82:
            output->key = KEYCODE_1;
            output->pressed = false;
            output->ch = '1';
            return false;

        case 0x83:
            output->key = KEYCODE_2;
            output->pressed = false;
            output->ch = '2';
            return false;

        case 0x84:
            output->key = KEYCODE_3;
            output->pressed = false;
            output->ch = '3';
            return false;

        case 0x85:
            output->key = KEYCODE_4;
            output->pressed = false;
            output->ch = '4';
            return false;

        case 0x86:
            output->key = KEYCODE_5;
            output->pressed = false;
            output->ch = '5';
            return false;

        case 0x87:
            output->key = KEYCODE_6;
            output->pressed = false;
            output->ch = '6';
            return false;

        case 0x88:
            output->key = KEYCODE_7;
            output->pressed = false;
            output->ch = '7';
            return false;

        case 0x89:
            output->key = KEYCODE_8;
            output->pressed = false;
            output->ch = '8';
            return false;

        case 0x8A:
            output->key = KEYCODE_9;
            output->pressed = false;
            output->ch = '9';
            return false;

        case 0xA9:
            output->key = KEYCODE_GRAVE;
            output->pressed = false;
            output->ch = output->shift ? '~' : '`';
            return false;

        case 0x8C:
            output->key = KEYCODE_MINUS;
            output->pressed = false;
            output->ch = output->shift ? '_' : '-';
            return false;

        case 0x8D:
            output->key = KEYCODE_EQUALS;
            output->pressed = false;
            output->ch = output->shift ? '+' : '=';
            return false;

        case 0xAB:
            output->key = KEYCODE_BACKSLASH;
            output->pressed = false;
            output->ch = output->shift ? '|' : '\\';
            return false;

        case 0x8E:
            output->key = KEYCODE_BACKSPACE;
            output->pressed = false;
            output->ch = 8;
            return false;

        case 0xB9:
            output->key = KEYCODE_SPACE;
            output->pressed = false;
            output->ch = ' ';
            return false;

        case 0x8F:
            output->key = KEYCODE_TAB;
            output->pressed = false;
            output->ch = '\t';
            return false;

        case 0xBA:
            output->key = KEYCODE_CAPSLOCK;
            output->pressed = false;
            output->ch = 0;
            return false;

        case 0xAA:
            output->key = KEYCODE_LEFT_SHIFT;
            output->pressed = false;
            output->ch = 0;
            output->shift = false;
            return false;

        case 0x9D:
            output->key = KEYCODE_LEFT_CONTROL;
            output->pressed = false;
            output->ch = 0;
            return false;

        case 0xB8:
            output->key = KEYCODE_LEFT_ALT;
            output->pressed = false;
            output->ch = 0;
            return false;

        case 0xB6:
            output->key = KEYCODE_RIGHT_SHIFT;
            output->pressed = false;
            output->ch = 0;
            output->shift = false;
            return false;

        case 0x9C:
            output->key = KEYCODE_ENTER;
            output->pressed = false;
            output->ch = '\n';
            return false;

        case 0x81:
            output->key = KEYCODE_ESCAPE;
            output->pressed = false;
            output->ch = 27;
            return false;

        case 0xBB:
            output->key = KEYCODE_F1;
            output->pressed = false;
            output->ch = 0;
            return false;

        case 0xBC:
            output->key = KEYCODE_F2;
            output->pressed = false;
            output->ch = 0;
            return false;

        case 0xBD:
            output->key = KEYCODE_F3;
            output->pressed = false;
            output->ch = 0;
            return false;

        case 0xBE:
            output->key = KEYCODE_F4;
            output->pressed = false;
            output->ch = 0;
            return false;

        case 0xBF:
            output->key = KEYCODE_F5;
            output->pressed = false;
            output->ch = 0;
            return false;

        case 0xC0:
            output->key = KEYCODE_F6;
            output->pressed = false;
            output->ch = 0;
            return false;

        case 0xC1:
            output->key = KEYCODE_F7;
            output->pressed = false;
            output->ch = 0;
            return false;

        case 0xC2:
            output->key = KEYCODE_F8;
            output->pressed = false;
            output->ch = 0;
            return false;

        case 0xC3:
            output->key = KEYCODE_F9;
            output->pressed = false;
            output->ch = 0;
            return false;

        case 0xC4:
            output->key = KEYCODE_F10;
            output->pressed = false;
            output->ch = 0;
            return false;

        case 0xD7:
            output->key = KEYCODE_F11;
            output->pressed = false;
            output->ch = 0;
            return false;

        case 0xD8:
            output->key = KEYCODE_F12;
            output->pressed = false;
            output->ch = 0;
            return false;

        case 0xC6:
            output->key = KEYCODE_SCROLL;
            output->pressed = false;
            output->ch = 0;
            return false;

        case 0x9A:
            output->key = KEYCODE_OPEN_BRACKET;
            output->pressed = false;
            output->ch = output->shift ? '{' : '[';
            return false;

        case 0xC5:
            output->key = KEYCODE_NUM;
            output->pressed = false;
            output->ch = 0;
            return false;

        case 0xB7:
            output->key = KEYCODE_NUMPAD_ASTERISK;
            output->pressed = false;
            output->ch = '*';
            return false;

        case 0xCA:
            output->key = KEYCODE_NUMPAD_MINUS;
            output->pressed = false;
            output->ch = '-';
            return false;

        case 0xCE:
            output->key = KEYCODE_NUMPAD_PLUS;
            output->pressed = false;
            output->ch = '+';
            return false;

        case 0xD3:
            output->key = KEYCODE_NUMPAD_PERIOD;
            output->pressed = false;
            output->ch = '.';
            return false;

        case 0xD2:
            output->key = KEYCODE_NUMPAD_0;
            output->pressed = false;
            output->ch = '0';
            return false;

        case 0xCF:
            output->key = KEYCODE_NUMPAD_1;
            output->pressed = false;
            output->ch = '1';
            return false;

        case 0xD0:
            output->key = KEYCODE_NUMPAD_2;
            output->pressed = false;
            output->ch = '2';
            return false;

        case 0xD1:
            output->key = KEYCODE_NUMPAD_3;
            output->pressed = false;
            output->ch = '3';
            return false;

        case 0xCB:
            output->key = KEYCODE_NUMPAD_4;
            output->pressed = false;
            output->ch = '4';
            return false;

        case 0xCC:
            output->key = KEYCODE_NUMPAD_5;
            output->pressed = false;
            output->ch = '5';
            return false;

        case 0xCD:
            output->key = KEYCODE_NUMPAD_6;
            output->pressed = false;
            output->ch = '6';
            return false;

        case 0xC7:
            output->key = KEYCODE_NUMPAD_7;
            output->pressed = false;
            output->ch = '7';
            return false;

        case 0xC8:
            output->key = KEYCODE_NUMPAD_8;
            output->pressed = false;
            output->ch = '8';
            return false;

        case 0xC9:
            output->key = KEYCODE_NUMPAD_9;
            output->pressed = false;
            output->ch = '9';
            return false;

        case 0x9B:
            output->key = KEYCODE_CLOSE_BRACKET;
            output->pressed = false;
            output->ch = output->shift ? '}' : ']';
            return false;

        case 0xA7:
            output->key = KEYCODE_SEMICOLON;
            output->pressed = false;
            output->ch = output->shift ? ':' : ';';
            return false;

        case 0xA8:
            output->key = KEYCODE_QUOTE;
            output->pressed = false;
            output->ch = output->shift ? '"' : '\'';
            return false;

        case 0xB3:
            output->key = KEYCODE_COMMA;
            output->pressed = false;
            output->ch = output->shift ? '<' : ',';
            return false;

        case 0xB4:
            output->key = KEYCODE_PERIOD;
            output->pressed = false;
            output->ch = output->shift ? '>' : '.';
            return false;

        case 0xB5:
            output->key = KEYCODE_SLASH;
            output->pressed = false;
            output->ch = output->shift ? '?' : '/';
            return false;

        case 0xE0:
            *state = PROCESSING_ESCAPE_CODE;
            return true;

        default:
            // Unknown scancode sequence.
            // Clear the input buffer to recover for next time.
            keyboard_clear_input();
            return true;
    };
}

static bool keyboard_step_esc(unsigned scancode, keyboard_output_t *output)
{
    switch (scancode) {
        case 0x5B:
            output->key = KEYCODE_LEFT_GUI;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x1D:
            output->key = KEYCODE_RIGHT_CONTROL;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x5C:
            output->key = KEYCODE_RIGHT_GUI;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x38:
            output->key = KEYCODE_RIGHT_ALT;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x5D:
            output->key = KEYCODE_APPS;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x52:
            output->key = KEYCODE_INSERT;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x47:
            output->key = KEYCODE_HOME;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x49:
            output->key = KEYCODE_PAGE_UP;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x53:
            output->key = KEYCODE_DELETE;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x4F:
            output->key = KEYCODE_END;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x51:
            output->key = KEYCODE_PAGE_DOWN;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x48:
            output->key = KEYCODE_UP_ARROW;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x4B:
            output->key = KEYCODE_LEFT_ARROW;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x50:
            output->key = KEYCODE_DOWN_ARROW;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x4D:
            output->key = KEYCODE_RIGHT_ARROW;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x35:
            output->key = KEYCODE_NUMPAD_SLASH;
            output->pressed = true;
            output->ch = output->shift ? '|' : '\\';
            return false;

        case 0x1C:
            output->key = KEYCODE_NUMPAD_ENTER;
            output->pressed = true;
            output->ch = '\n';
            return false;

        case 0x5E:
            output->key = KEYCODE_POWER;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x5F:
            output->key = KEYCODE_SLEEP;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x63:
            output->key = KEYCODE_WAKE;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x19:
            output->key = KEYCODE_NEXT_TRACK;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x10:
            output->key = KEYCODE_PREV_TRACK;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x24:
            output->key = KEYCODE_STOP;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x22:
            output->key = KEYCODE_PLAY_PAUSE;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x20:
            output->key = KEYCODE_MUTE;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x30:
            output->key = KEYCODE_VOLUME_UP;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x2E:
            output->key = KEYCODE_VOLUME_DOWN;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x6D:
            output->key = KEYCODE_MEDIA_SELECT;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x6C:
            output->key = KEYCODE_EMAIL;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x21:
            output->key = KEYCODE_CALCULATOR;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x6B:
            output->key = KEYCODE_MY_COMPUTER;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x65:
            output->key = KEYCODE_WWW_SEARCH;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x32:
            output->key = KEYCODE_WWW_HOME;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x6A:
            output->key = KEYCODE_WWW_BACK;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x69:
            output->key = KEYCODE_WWW_FORWARD;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x68:
            output->key = KEYCODE_WWW_STOP;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x67:
            output->key = KEYCODE_WWW_REFRESH;
            output->pressed = true;
            output->ch = 0;
            return false;

        case 0x66:
            output->key = KEYCODE_WWW_FAVORITES;
            output->pressed = true;
            output->ch = 0;
            return false;

        default:
            // Unknown scancode sequence.
            // Clear the input buffer to recover for next time.
            keyboard_clear_input();
            return true;
    };
}

static bool keyboard_step_state_machine(keyboard_state_t *state, keyboard_output_t *output)
{
    unsigned scancode = inb(KEYBOARD_DATA_PORT);

    switch (*state) {
        case IDLE: return keyboard_step_idle(scancode, state, output);
        case PROCESSING_ESCAPE_CODE: return keyboard_step_esc(scancode, output);
        default:
            panic("Keyboard driver is in an unknown state: %d", *state);
            return true;
    }
}

void keyboard_int_handler()
{
    keyboard_state_t state = IDLE;
    keyboard_output_t output = {
        .pressed = false,
        .key = KEYCODE_MAX,
        .ch = 0
    };

    for (int i = 0; i < MAX_SCANCODE_BYTES && keyboard_step_state_machine(&state, &output); ++i);

    if (output.pressed && (isprint(output.ch) || (output.ch == '\n') || (output.ch == '\t'))) {
        console_putchar(output.ch);
    }
}

void keyboard_init()
{
    // Do nothing
}
