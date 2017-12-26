#pragma once

#include <cstddef>

#include <common/linked_list.hpp>
#include <common/vector.hpp>
#include <common/text_terminal.hpp>
#include <common/keyboard_device.hpp>

// This can be used to read lines of user input from the terminal and provides
// useful UI affordances such as nice editing functionality, a prompt, history,
class line_editor {
    text_terminal &term;
    keyboard_device &kb;
    vector<char> prompt;
    linked_list<vector<char>> history;

public:
    ~line_editor();

    // Constructor.
    // and such.
    // display -- The text terminal where we display characters.
    // kb -- The keyboard device which gives us user input.
    line_editor(text_terminal &term, keyboard_device &kb);

    // Prompt for one line of user input on the console.
    // Returns a string containing the line of user input.
    vector<char> getline();

    // Change the prompt displayed at the beginning of the line.
    void set_prompt(vector<char> new_prompt);

    // Add a line to the editor's history.
    void add_history(vector<char> history_entry);
};
