#pragma once

#include <common/vector.hpp>

class text_display_device;

class text_line {
    vector<char> _data;
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
    void draw(text_display_device &display, int row) const;

    // Count the columns needed to represent the text line.
    int columns() const;

    // Gets the characters in the text line.
    // The string is not nul-terminated.
    const vector<char>& get() const;

    // Gets the characters in the text line.
    // The string is nul-terminated.
    vector<char> str() const;

    // Inserts a character at the end of the the line.
    // If the character won't fit on the line then the line remains unmodified
    // and this call returns false.
    bool push_back(char ch);
};
