#pragma once

#include <stddef.h>

// Prompt for one line of user input on the console.
// Returns the number of characters placed into the buffer.
// prompt -- Prompt string to display on the console.
// count -- Maximum number of bytes in buffer.
// buffer -- The output string is written here.
size_t readline(const char * const prompt, size_t count, char *buffer);
