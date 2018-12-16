#include <drivers/pc/ps2_keyboard_device.hpp>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <cassert>

#include <interrupt_asm.h>
#include <halt.h>
#include <inout.h>

constexpr unsigned KEYBOARD_DATA_PORT = 0x60;
//constexpr unsigned KEYBOARD_CONTROL_PORT = 0x64;
constexpr unsigned MAX_SCANCODE_BYTES = 6;

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

const char* PS2KeyboardDevice::keycode_name(Keycode key)
{
    if (key < KEYCODE_MAX) {
        return g_keycode_names[key];
    } else {
        return "Unknown Key Code";
    }
}

void PS2KeyboardDevice::clear_input(void)
{
    for (size_t i = 0; i < MAX_SCANCODE_BYTES; ++i) {
        (void)inb(KEYBOARD_DATA_PORT);
    }
}

bool PS2KeyboardDevice::step_state_machine(KeyboardDriverState &state, KeyboardEvent &output)
{
    unsigned scancode = inb(KEYBOARD_DATA_PORT);

    bool escape = (state == PROCESSING_ESCAPE_CODE);

    if (scancode == 0xE0) {
        state = PROCESSING_ESCAPE_CODE;
        return true;
    }

    const uint8_t *scancode_sets[] = {
        escape ? g_scancodes_make_escaped : g_scancodes_make,
        escape ? g_scancodes_break_escape : g_scancodes_break
    };

    const KeycodeKeyState states[] = { PRESSED, RELEASED };

    for(size_t i = 0, n = sizeof(scancode_sets) / sizeof(*scancode_sets); i < n; ++i) {
        const uint8_t *scancodes = scancode_sets[i];

        for (Keycode key = 0; key < KEYCODE_MAX; ++key) {
            if (scancode == scancodes[key]) {
                output.key = key;
                output.state = states[i];
                return false;
            }
        }
    }

    // Unknown scancode sequence.
    // Clear the input buffer to recover for next time.
    clear_input();
    return true;
}

void PS2KeyboardDevice::int_handler(const ParameterPackage&)
{
    KeyboardDriverState state = IDLE;
    KeyboardEvent event;

    for (size_t i = 0; i < MAX_SCANCODE_BYTES && step_state_machine(state, event); ++i);

    if (event.key < KEYCODE_MAX) {
        key_state_[event.key] = event.state;

        bool shift = (key_state_[KEYCODE_LEFT_SHIFT] == PRESSED) || (key_state_[KEYCODE_RIGHT_SHIFT] == PRESSED);
        const char* char_tbl = shift ? g_keyboard_keycode_ascii_uppercase : g_keyboard_keycode_ascii_lowercase;
        event.ch = char_tbl[event.key];

        // No synchronization is needed here. We're synchronizing access to the
        // ring buffer only by disabling interrupts, and interrupts are already
        // disabled. As there is only one CPU (in use, at least) the only way a
        // new thread can execute is through an interrupt which invokes the
        // scheduler or something like that.
        // XXX: Need better kernel synchronization primitives.
        events_.push_back(event);
    }
}

KeyboardEvent PS2KeyboardDevice::get_event()
{
    KeyboardEvent event;

    bool have_a_key = false;

    while (!have_a_key) {
        // Until there are better synchronization primitives, simply disable
        // interrupts while reading the ring buffer. As there is only one CPU
        // (in use, at least) the only way a new thread can execute is through
        // an interrupt which invokes the scheduler or something like that.
        // XXX: Need better kernel synchronization primitives.
        disable_interrupts();
        have_a_key = !events_.empty();
        if (have_a_key) {
            event = events_.front();
            events_.pop_front();
        }
        enable_interrupts();
        hlt();
    }

    return event;
}

PS2KeyboardDevice::PS2KeyboardDevice()
{
    for (size_t i = 0; i < KEYCODE_MAX; ++i) {
        key_state_[i] = RELEASED;
    }
}

PS2KeyboardDevice::~PS2KeyboardDevice() noexcept = default;
