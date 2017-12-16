#pragma once

#include <stddef.h>
#include <interfaces/console_interface.h>
#include <interfaces/keyboard_interface.h>

// Prompt for one line of user input on the console.
// Returns the number of characters placed into the buffer.
// prompt -- Prompt string to display on the console.
// count -- Maximum number of bytes in buffer.
// buffer -- The output string is written here.
size_t readline(const console_interface_t *console,
                const keyboard_interface_t *keyboard,
                size_t prompt_size, const char * const prompt,
                size_t buffer_size, char *buffer);