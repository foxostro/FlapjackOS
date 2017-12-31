#pragma once

#include <common/ring_buffer.hpp>
#include <common/text_line.hpp>

class text_display_device;

// A text terminal displays lines of text on a text console display.
class text_terminal {
    // Displays monospace text on the screen.
    text_display_device &_display;

    // Logical lines of text in the terminal. Each of these may span one or more
    // physical lines on the display if they're too long to fit.
    ring_buffer<text_line, CONSOLE_HEIGHT> _logical_lines;

    // The position of the cursor in logical space.
    int _logical_cursor_row;
    //int _logical_cursor_col

    void _putchar(char ch);

public:
    ~text_terminal();

    // Constructor.
    text_terminal(text_display_device &display);

    // Draws the terminal on the display.
    void draw();

    // Puts a character on the display at the current cursor position.
    // Increments the cursor to the next position. This may wrap the cursor to
    // the next line. If the cursor reaches the bottom of the console display
    // then lines of text will scroll up off the top.
    //
    // - Many non-printable characters are ignored.
    // - Tab characters ('\t') may be wider than one column.
    // - The backspace character ('\b') deletes a character from the terminal
    //   and moves the cursor to the previous position.
    void putchar(char ch);

    // Puts each character in the vector to the terminal.
    void puts(const char *str);

    // Puts each character in the string to the terminal.
    void puts(const vector<char> &str);

    // Prints a formatted string to the terminal.
    int printf(const char *fmt, ...);

    // Returns the logical lines in the text terminal. Each of these may span
    // one or more physical lines on the display if they're too long to fit.
    const ring_buffer<text_line, CONSOLE_HEIGHT>& get_logical_lines() const
    {
        return _logical_lines;
    }
};
