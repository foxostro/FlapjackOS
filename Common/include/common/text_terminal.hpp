#pragma once

#include <common/text_display_device.hpp>
#include <common/text_line.hpp>
#include <common/vector.hpp>

// A text terminal displays lines of text on a text console display.
class text_terminal {
    // Displays monospace text on the screen.
    text_display_device &display;

    // Lines of text in the terminal.
    vector<text_line> lines;

    // The row and column of the display where the cursor sits.
    int cursor_row, cursor_col;

    // Increments the cursor appropriately for typing the specified character
    // at the specified row and column of the terminal.
    void increment_cursor(int &row, int &col, char ch);

    // Repaint the specified line of text on the specified row of the display.
    void repaint_line(int row, text_line &line);

public:
    ~text_terminal();

    // Constructor.
    text_terminal(text_display_device &display);

    // Inserts a character on the terminal at the specified row and column.
    // This may cause characters on the terminal following this position to be
    // displaced.
    //
    // The cursor is unaffected.
    //
    // - Non-printable characters are ignored.
    // - Tab characters ('\t') may be wider than one column.
    // - The backspace character ('\b') deletes a line of text from the console
    //   and moves the cursor to the previous position.
    void insert_char(int row, int col, char ch);

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
};
