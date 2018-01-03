#pragma once

#include <cstddef>

#include <common/linked_list.hpp>
#include <common/text_terminal.hpp>
#include <common/keyboard_device.hpp>

// Maxiumum size of a prompt in line_editor.
static constexpr int MAXPROMPT = 16;

// This can be used to read lines of user input from the terminal and provides
// useful UI affordances such as nice editing functionality, a prompt, history,
class line_editor {
public:
    using line_t = ring_buffer<char, MAXLINE>;

    ~line_editor();

    // Constructor.
    // and such.
    // display -- The text terminal where we display characters.
    // kb -- The keyboard device which gives us user input.
    line_editor(text_terminal &term, keyboard_device &kb);

    // Prompt for one line of user input on the console.
    // Returns a string containing the line of user input.
    line_t getline();

    // Change the prompt displayed at the beginning of the line.
    void set_prompt(size_t size, const char *prompt);

    // Add a line to the editor's history.
    void add_history(line_t entry);

private:
    text_terminal &term;
    keyboard_device &kb;
    ring_buffer<char, MAXPROMPT> prompt;
    linked_list<line_t> history;
};
