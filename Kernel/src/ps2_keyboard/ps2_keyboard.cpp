#include <ps2_keyboard.hpp>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <inout.h>
#include <console.h>
#include <ctypes.h>
#include <interrupt_asm.h>
#include <halt.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

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

void ps2_keyboard::enqueue(keyboard_event_t *event)
{
    // Drop keyboard input if the buffer is full.
    if (count < BUFFER_SIZE) {
        memcpy(&ring_buffer[enqueue_pos], event, sizeof(keyboard_event_t));
        enqueue_pos++;

        if (enqueue_pos >= BUFFER_SIZE) {
            enqueue_pos = 0;
        }

        count++;
    }
}

bool ps2_keyboard::dequeue(keyboard_event_t *event)
{
    if(count == 0) {
        return false;
    }

    count--;

    memcpy(event, &ring_buffer[dequeue_pos], sizeof(keyboard_event_t));
    dequeue_pos++;

    if(dequeue_pos >= BUFFER_SIZE) {
        dequeue_pos = 0;
    }

    return true;
}

const char* ps2_keyboard::keycode_name(keycode_t key)
{
    if (key < KEYCODE_MAX) {
        return g_keycode_names[key];
    } else {
        return "Unknown Key Code";
    }
}

void ps2_keyboard::clear_input(void)
{
    for (size_t i = 0; i < MAX_SCANCODE_BYTES; ++i) {
        (void)inb(KEYBOARD_DATA_PORT);
    }
}

bool ps2_keyboard::step_state_machine(keyboard_driver_state_t *state, keyboard_event_t *output)
{
    assert(state);
    assert(output);

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
    clear_input();
    return true;
}

void ps2_keyboard::int_handler()
{
    keyboard_driver_state_t state = IDLE;

    keyboard_event_t event;
    event.state = RELEASED;
    event.key = KEYCODE_MAX;

    for (int i = 0; i < MAX_SCANCODE_BYTES && step_state_machine(&state, &event); ++i);

    if (event.key < KEYCODE_MAX) {
        key_state[event.key] = event.state;

        bool shift = (key_state[KEYCODE_LEFT_SHIFT] == PRESSED) || (key_state[KEYCODE_RIGHT_SHIFT] == PRESSED);
        const char* char_tbl = shift ? g_keyboard_keycode_ascii_uppercase : g_keyboard_keycode_ascii_lowercase;
        event.ch = char_tbl[event.key];

        // No synchronization is needed here. We're synchronizing access to the
        // ring buffer only by disabling interrupts, and interrupts are already
        // disabled. As there is only one CPU (in use, at least) the only way a
        // new thread can execute is through an interrupt which invokes the
        // scheduler or something like that.
        // XXX: Need better kernel synchronization primitives.
        enqueue(&event);
    }
}

void ps2_keyboard::get_event(keyboard_event_t *event)
{
    assert(event);

    bool have_a_key = false;

    while (!have_a_key) {
        // Until there are better synchronization primitives, simply disable
        // interrupts while reading the ring buffer. As there is only one CPU
        // (in use, at least) the only way a new thread can execute is through
        // an interrupt which invokes the scheduler or something like that.
        // XXX: Need better kernel synchronization primitives.
        disable_interrupts();
        have_a_key = dequeue(event);
        enable_interrupts();
        hlt();
    }
}

ps2_keyboard::ps2_keyboard()
 : dequeue_pos(0),
   enqueue_pos(0),
   count(0)
{
    memset(ring_buffer, 0, sizeof(ring_buffer));

    for (size_t i = 0; i < KEYCODE_MAX; ++i) {
        key_state[i] = RELEASED;
    }
}
