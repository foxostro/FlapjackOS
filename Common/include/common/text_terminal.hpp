#pragma once

#include <common/text_buffer.hpp>
#include <common/text_line.hpp>

class text_display_device;

// A text terminal displays lines of text on a text console display.
class text_terminal {
    // Displays monospace text on the screen.
    text_display_device &_display;

    // Text in the terminal, and associated cursor.
    text_buffer _buffer;

public:
    ~text_terminal();

    // Constructor.
    text_terminal(text_display_device &display);

    // Draws the terminal on the display.
    void draw() const;

    // Format the text buffer into lines of text which fit on the display.
    // This may be more lines of text than can fit into the display vertically.
    vector<text_line> get_lines() const;

    // Puts a character on the display at the current cursor position.
    // Increments the cursor to the next position. This may wrap the cursor to
    // the next line. If the cursor reaches the bottom of the console display
    // then lines of text will scroll up off the top.
    //
    // - Non-printable characters are ignored.
    // - Tab characters ('\t') may be wider than one column.
    // - The backspace character ('\b') deletes a line of text from the console
    //   and moves the cursor to the previous position.
    void putchar(char ch);

    // Puts each character in the vector to the terminal.
    void puts(const char *str);

    // Puts each character in the string to the terminal.
    void puts(const vector<char> &str);

    // Prints a formatted string to the terminal.
    int printf(const char *fmt, ...);

    // Mostly useful for testing.
    text_buffer& get_buffer() { return _buffer; }
};
