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

static const uint8_t g_scancodes_make[KEYCODE_MAX] = {
#include "keyboard_scancodes_make.inc"
};

static const uint8_t g_scancodes_break[KEYCODE_MAX] = {
#include "keyboard_scancodes_break.inc"
};

static const uint8_t g_scancodes_make_escaped[KEYCODE_MAX] = {
#include "keyboard_scancodes_make_escaped.inc"
};

static const uint8_t g_scancodes_break_escape[KEYCODE_MAX] = {
#include "keyboard_scancodes_break_escaped.inc"
};

static const char* g_keycode_names[KEYCODE_MAX] = {
#include "keyboard_keycode_names.inc"
};

static const char g_keyboard_keycode_ascii_lowercase[KEYCODE_MAX] = {
#include "keyboard_keycode_ascii_lowercase.inc"
};

static const char g_keyboard_keycode_ascii_uppercase[KEYCODE_MAX] = {
#include "keyboard_keycode_ascii_uppercase.inc"
};

keycode_key_state_t g_key_state[KEYCODE_MAX];

typedef enum {
    IDLE,
    PROCESSING_ESCAPE_CODE
} keyboard_driver_state_t;

const char* keyboard_keycode_name(keycode_t key)
{
    if (key < KEYCODE_MAX) {
        return g_keycode_names[key];
    } else {
        return "Unknown Key Code";
    }
}

static void keyboard_clear_input()
{
    for (int i = 0; i < MAX_SCANCODE_BYTES; ++i) {
        (void)inb(KEYBOARD_DATA_PORT);       
    }
}

static bool keyboard_step_state_machine(keyboard_driver_state_t *state, keyboard_event_t *output)
{
    unsigned scancode = inb(KEYBOARD_DATA_PORT);

    bool escape = (*state == PROCESSING_ESCAPE_CODE);

    if (scancode == 0xE0) {
        *state = PROCESSING_ESCAPE_CODE;
        return true;
    }

    const uint8_t *scancode_sets[] = {
        escape ? g_scancodes_make_escaped : g_scancodes_make,
        escape ? g_scancodes_break_escape : g_scancodes_break
    };

    const keycode_key_state_t states[] = { PRESSED, RELEASED };

    for(size_t i = 0, n = sizeof(scancode_sets) / sizeof(*scancode_sets); i < n; ++i) {
        const uint8_t *scancodes = scancode_sets[i];

        for (keycode_t key = 0; key < KEYCODE_MAX; ++key) {
            if (scancode == scancodes[key]) {
                output->key = key;
                output->state = states[i];
                return false;
            }
        }
    }

    // Unknown scancode sequence.
    // Clear the input buffer to recover for next time.
    keyboard_clear_input();
    return true;
}

void keyboard_int_handler()
{
    keyboard_driver_state_t state = IDLE;
    keyboard_event_t event = { .state = RELEASED, .key = KEYCODE_MAX };

    for (int i = 0; i < MAX_SCANCODE_BYTES && keyboard_step_state_machine(&state, &event); ++i);

    if (event.key < KEYCODE_MAX) {
        g_key_state[event.key] = event.state;

        bool shift = (g_key_state[KEYCODE_LEFT_SHIFT] == PRESSED) || (g_key_state[KEYCODE_RIGHT_SHIFT] == PRESSED);
        const char* char_tbl = shift ? g_keyboard_keycode_ascii_uppercase : g_keyboard_keycode_ascii_lowercase;
        char c = char_tbl[event.key];

        if ((event.state == PRESSED) && console_is_acceptable(c)) {
            console_putchar(c);
        }
    }
}

void keyboard_init()
{
    for (size_t i = 0; i < KEYCODE_MAX; ++i) {
        g_key_state[i] = RELEASED;
    }
}
