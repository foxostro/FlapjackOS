#pragma once

#include <stddef.h>
#include <interfaces/console_interface.h>
#include <interfaces/keyboard_interface.h>
#include <interfaces/malloc_interface.h>

typedef struct readline_interface {
    // Frees memory associated with the readline object.
    void (*destroy)(struct readline_interface *this);

    // Prompt for one line of user input on the console.
    // Returns the number of characters placed into the buffer.
    // buffer_size -- The capacity of the buffer in `buffer'
    // buffer -- The output string is written here.
    size_t (*readline)(struct readline_interface *this,
                       size_t buffer_size,
                       char *buffer);
} readline_t;

// Initializes the readline object.
// allocator -- Memory allocator
// console -- The console where we display characters.
// keyboard -- The keyboard which gives us user input.
// prompt_size -- The capacity of the buffer in `prompt'
// prompt -- Prompt string to display on the console.
readline_t* readline_init(malloc_interface_t *allocator,
                          console_interface_t *console,
                          keyboard_interface_t *keyboard,
                          size_t prompt_size, const char *prompt);
