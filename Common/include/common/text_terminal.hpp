#pragma once

#include <common/console_device.hpp>
#include <common/vector.hpp>

// A text terminal displays lines of text on a text console display.
class text_terminal {
    static const int TAB_WIDTH = 8;
    console_device &display;
    int cursor_row, cursor_col;

    void move_back_one(int &row, int &col);
    void backspace(int &row, int &col);
    void newline(int &row, int &col);
    void enter_character(int &row, int &col, char ch);

public:
    ~text_terminal();

    // Constructor.
    text_terminal(console_device &display);

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
    void puts(vector<char> &str);

    // Prints a formatted string to the terminal.
    int printf(const char *fmt, ...);

    // Moves the cursor to the specified position on the display.
    void set_cursor_position(int row, int col);

    int get_cursor_row() const;
    int get_cursor_col() const;

    void move_cursor_left();
    void move_cursor_right();
};
