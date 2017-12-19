#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <inout.h>
#include <console.h>
#include <ctypes.h>
#include <ps2_keyboard.h>
#include <interrupt_asm.h>
#include <halt.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define KEYBOARD_DATA_PORT     (0x60)
#define KEYBOARD_CONTROL_PORT  (0x64)
#define MAX_SCANCODE_BYTES     (6)

typedef enum {
    IDLE,
    PROCESSING_ESCAPE_CODE
} keyboard_driver_state_t;

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

static void keyboard_enqueue(ps2_keyboard_t *self, keyboard_event_t *event)
{
    assert(self);

    // Drop keyboard input if the buffer is full.
    if (self->count < BUFFER_SIZE) {
        memcpy(&self->ring_buffer[self->enqueue_pos], event, sizeof(keyboard_event_t));
        self->enqueue_pos++;

        if (self->enqueue_pos >= BUFFER_SIZE) {
            self->enqueue_pos = 0;
        }

        self->count++;
    }
}

static bool keyboard_dequeue(ps2_keyboard_t *self, keyboard_event_t *event)
{
    assert(self);
    
    if(self->count == 0) {
        return false;
    }

    self->count--;

    memcpy(event, &self->ring_buffer[self->dequeue_pos], sizeof(keyboard_event_t));
    self->dequeue_pos++;

    if(self->dequeue_pos >= BUFFER_SIZE) {
        self->dequeue_pos = 0;
    }

    return true;
}

const char* keyboard_keycode_name(keycode_t key)
{
    if (key < KEYCODE_MAX) {
        return g_keycode_names[key];
    } else {
        return "Unknown Key Code";
    }
}

static void keyboard_clear_input(void)
{
    for (size_t i = 0; i < MAX_SCANCODE_BYTES; ++i) {
        (void)inb(KEYBOARD_DATA_PORT);       
    }
}

static bool keyboard_step_state_machine(keyboard_driver_state_t *state, keyboard_event_t *output)
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
    keyboard_clear_input();
    return true;
}

static void keyboard_destroy(ps2_keyboard_t *self)
{
    free(self);
}

static void keyboard_int_handler(ps2_keyboard_t *self)
{
    assert(self);
    
    keyboard_driver_state_t state = IDLE;
    keyboard_event_t event = { .state = RELEASED, .key = KEYCODE_MAX };

    for (int i = 0; i < MAX_SCANCODE_BYTES && keyboard_step_state_machine(&state, &event); ++i);

    if (event.key < KEYCODE_MAX) {
        self->key_state[event.key] = event.state;

        bool shift = (self->key_state[KEYCODE_LEFT_SHIFT] == PRESSED) || (self->key_state[KEYCODE_RIGHT_SHIFT] == PRESSED);
        const char* char_tbl = shift ? g_keyboard_keycode_ascii_uppercase : g_keyboard_keycode_ascii_lowercase;
        event.ch = char_tbl[event.key];

        // No synchronization is needed here. We're synchronizing access to the
        // ring buffer only by disabling interrupts, and interrupts are already
        // disabled. As there is only one CPU (in use, at least) the only way a
        // new thread can execute is through an interrupt which invokes the
        // scheduler or something like that.
        // XXX: Need better kernel synchronization primitives.
        keyboard_enqueue(self, &event);
    }
}

static void keyboard_get_event(ps2_keyboard_t *self, keyboard_event_t *event)
{
    assert(self);
    assert(event);
    
    bool have_a_key = false;

    while (!have_a_key) {
        // Until there are better synchronization primitives, simply disable
        // interrupts while reading the ring buffer. As there is only one CPU
        // (in use, at least) the only way a new thread can execute is through
        // an interrupt which invokes the scheduler or something like that.
        // XXX: Need better kernel synchronization primitives.
        disable_interrupts();
        have_a_key = keyboard_dequeue(self, event);
        enable_interrupts();
        hlt();
    }
}

keyboard_interface_t* ps2_keyboard_init(void)
{
    ps2_keyboard_t *self = malloc(sizeof(ps2_keyboard_t));
    memset(self, 0, sizeof(ps2_keyboard_t));

    self->destroy = keyboard_destroy;
    self->int_handler = keyboard_int_handler;
    self->keycode_name = keyboard_keycode_name;
    self->get_event = keyboard_get_event;
    
    for (size_t i = 0; i < KEYCODE_MAX; ++i) {
        self->key_state[i] = RELEASED;
    }

    return (keyboard_interface_t *)self;
}
