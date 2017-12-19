#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <ll_str.h>
#include <line_editor.h>

typedef struct line_editor_impl {
    void (*destroy)(struct line_editor_impl *self);
    char* (*getline)(struct line_editor_impl *self);
    void (*set_prompt)(struct line_editor_impl *self, size_t prompt_size, const char *prompt);
    void (*add_history)(struct line_editor_impl *self, const char *history);

    console_interface_t *console;
    keyboard_interface_t *keyboard;
    size_t prompt_size;
    char *prompt;
    ll_str_t *history;
    int history_cursor;
} line_editor_impl_t;

#ifdef __cplusplus
}
#endif