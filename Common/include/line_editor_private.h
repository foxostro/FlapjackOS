#pragma once

#include <stddef.h>
#include <line_editor.h>

typedef struct line_editor_impl {
    void (*destroy)(struct line_editor_impl *this);
    char* (*getline)(struct line_editor_impl *this);
    void (*set_prompt)(struct line_editor_impl *this, size_t prompt_size, const char *prompt);

    console_interface_t *console;
    keyboard_interface_t *keyboard;
    size_t prompt_size;
    char *prompt;
} line_editor_impl_t;

_Static_assert(offsetof(line_editor_impl_t, destroy) == offsetof(line_editor_t, destroy),
    "Incorrect offset prevents correct casting behavior when casting between types.");

_Static_assert(offsetof(line_editor_impl_t, getline) == offsetof(line_editor_t, getline),
    "Incorrect offset prevents correct casting behavior when casting between types.");

_Static_assert(offsetof(line_editor_impl_t, set_prompt) == offsetof(line_editor_t, set_prompt),
    "Incorrect offset prevents correct casting behavior when casting between types.");
