#pragma once

#include <stddef.h>
#include <interfaces/console_interface.h>
#include <interfaces/keyboard_interface.h>
#include <interfaces/malloc_interface.h>

typedef struct readline_interface {
    // Frees memory associated with the readline object.
    void (*destroy)(struct readline_interface *this);

    // Prompt for one line of user input on the console.
    // Returns a string containing the line of user input.
    // It is the responsibility of the caller to free this string when finished.
    // It will have been allocated using the allocator originally passed to
    // readline_init().
    char* (*readline)(struct readline_interface *this);

    // Change the prompt displayed at the beginning of the line.
    // The specified string is copied and so the buffer is not required to live
    // past the call to this method.
    void (*set_prompt)(struct readline_interface *this,
                       size_t prompt_size,
                       const char *prompt);
} readline_t;

// Returns a new initialized readline object.
// allocator -- Memory allocator
// console -- The console where we display characters.
// keyboard -- The keyboard which gives us user input.
// prompt_size -- The capacity of the buffer in `prompt'
// prompt -- Prompt string to display on the console.
readline_t* readline_init(malloc_interface_t *allocator,
                          console_interface_t *console,
                          keyboard_interface_t *keyboard);
