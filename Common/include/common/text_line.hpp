#pragma once

#include <common/text_buffer.hpp>

class text_display_device;

class text_line {
    const text_buffer *_owner;
    int _tab_width;

public:
    // The number of columns this character will take up.
    static int step_for_char(int tab_width, int col, char ch);

    // Constructor.
    text_line(const text_buffer *owner, int tab_width);

    // Draws the text line at the specified line of the display.
    void draw(text_display_device &display, int row) const;

    // Count the columns needed to represent the text line.
    int columns() const;

    // Gets the characters in the text line.
    // A nul-terminator is added at the end of the vector.
    vector<char> get() const;

    text_buffer::cursor_position begin;
    text_buffer::cursor_position end;
};
