#pragma once

#include <common/ring_buffer.hpp>
#include <common/vector.hpp>
#include <common/terminal_metrics.hpp>

class text_display_device;

class text_line {
    ring_buffer<char, MAXLINE> _data;
    int _max_columns;
    int _tab_width;

public:
    // The number of columns this character will take up.
    static int step_for_char(int tab_width, int col, char ch);

    // Destructor.
    ~text_line();

    // Constructor.
    text_line();

    // Constructor. The line will be enforced to be no more than the
    // specified number of columns wide.
    text_line(int max_columns, int tab_width);

    // Copy constructor.
    text_line(const text_line &line);

    // Move constructor.
    text_line(text_line &&line);

    // Copy-assignment operator.
    text_line& operator=(const text_line &other);

    // Move-assignment operator.
    text_line& operator=(text_line &&other);

    // Draws the text line at the specified line of the display.
    // Returns the display line where the next logical line should be placed.
    int draw(text_display_device &display, int row);

    // Count the number of columns and rows on the display needed to display the
    // text line.
    void measure(int &out_rows, int &out_columns) const;

    // Gets the characters in the text line.
    // The string is nul-terminated.
    vector<char> str() const;

    // Inserts a character at the end of the line.
    // If the character won't fit on the line then the line remains unmodified
    // and this call returns false.
    // The character must not be a newline, '\n'.
    bool push_back(char ch);

    // Indicates line needs to be redrawn.
    bool dirty;
};
