#pragma once

#include <cstddef>

#include <common/linked_list.hpp>
#include <common/console_device.hpp>
#include <common/keyboard_device.hpp>

// This can be used to read lines of user input from the terminal and provides
// useful UI affordances such as nice editing functionality, a prompt, history,
class line_editor {
    console_device &con;
    keyboard_device &kb;
    size_t prompt_size;
    char *prompt;
    linked_list<char*> history;
    int history_cursor;

    void backspace(char *buffer,
                   size_t &count,
                   size_t &cursor_col,
                   size_t cursor_row,
                   size_t cursor_col_offset);

    void type_character(char *buffer,
                        size_t maxcount,
                        size_t &count,
                        size_t &cursor_col,
                        size_t cursor_row,
                        size_t cursor_col_offset,
                        char ch);

    void replace_entire_line(char *replacement,
                             char *buffer,
                             size_t maxcount,
                             size_t &count,
                             size_t &cursor_col,
                             size_t cursor_row,
                             size_t cursor_col_offset);

public:
    ~line_editor();

    // Constructor.
    // and such.
    // con -- The console device where we display characters.
    // kb -- The keyboard device which gives us user input.
    line_editor(console_device &con, keyboard_device &kb);

    // Prompt for one line of user input on the console.
    // Returns a string containing the line of user input.
    // The string will have been allocated via malloc.
    // It is the responsibility of the caller to free this string when finished.
    char* getline();

    // Change the prompt displayed at the beginning of the line.
    // The specified string is copied and so the buffer is not required to live
    // past the call to this method.
    void set_prompt(size_t prompt_size, const char *prompt);

    // Add a line to the editor history.
    void add_history(const char *history);
};
