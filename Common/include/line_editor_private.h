#pragma once

#include <stddef.h>
#include <ll_str.h>
#include <line_editor.h>

typedef struct line_editor_impl {
    void (*destroy)(struct line_editor_impl *this);
    char* (*getline)(struct line_editor_impl *this);
    void (*set_prompt)(struct line_editor_impl *this, size_t prompt_size, const char *prompt);
    void (*add_history)(struct line_editor_impl *this, const char *history);

    console_interface_t *console;
    keyboard_interface_t *keyboard;
    size_t prompt_size;
    char *prompt;
    ll_str_t *history;
    int history_cursor;
} line_editor_impl_t;

_Static_assert(offsetof(line_editor_impl_t, destroy) == offsetof(line_editor_t, destroy),
    "Incorrect offset prevents correct casting behavior when casting between types.");

_Static_assert(offsetof(line_editor_impl_t, getline) == offsetof(line_editor_t, getline),
    "Incorrect offset prevents correct casting behavior when casting between types.");

_Static_assert(offsetof(line_editor_impl_t, set_prompt) == offsetof(line_editor_t, set_prompt),
    "Incorrect offset prevents correct casting behavior when casting between types.");

_Static_assert(offsetof(line_editor_impl_t, add_history) == offsetof(line_editor_t, add_history),
    "Incorrect offset prevents correct casting behavior when casting between types.");
