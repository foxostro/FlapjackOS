#pragma once

#include <stddef.h>
#include <readline.h>

typedef struct readline_impl {
    void (*destroy)(struct readline_impl *this);
    char* (*readline)(struct readline_impl *this);
    void (*set_prompt)(struct readline_impl *this, size_t prompt_size, const char *prompt);

    malloc_interface_t *allocator;
    console_interface_t *console;
    keyboard_interface_t *keyboard;
    size_t prompt_size;
    char *prompt;
} readline_impl_t;

_Static_assert(offsetof(readline_impl_t, destroy) == offsetof(readline_t, destroy),
    "Incorrect offset prevents correct casting behavior when casting between types.");

_Static_assert(offsetof(readline_impl_t, readline) == offsetof(readline_t, readline),
    "Incorrect offset prevents correct casting behavior when casting between types.");

_Static_assert(offsetof(readline_impl_t, set_prompt) == offsetof(readline_t, set_prompt),
    "Incorrect offset prevents correct casting behavior when casting between types.");
