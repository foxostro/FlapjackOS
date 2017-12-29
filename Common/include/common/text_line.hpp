#pragma once

#include <common/vector.hpp>

class text_line {
    vector<char> _data;
    const int _max_columns;
    const int _tab_width;

public:
    // The number of columns this character will take up.
    static int step_for_char(int tab_width, int col, char ch);

    // Constructor.
    text_line();

    // Constructor. The line will be enforced to be no more than the
    // specified number of columns wide.
    text_line(int max_columns, int tab_width);

    // Returns the characters in the line of text.
    const vector<char>& data() const;

    // Returns the number of columns.
    int columns() const;

    // Call the specified callable `fn' for each column in the line.
    // Returns the number of columns processed.
    // fn -- void(int column, int width, char ch)
    template<typename F>
    int for_each_col(F &&fn) const
    {
        int col = 0;
        for (int i = 0; i < _data.size(); ++i) {
            const char ch = _data[i];
            const int delta = step_for_char(_tab_width, col, ch);
            fn(col, delta, ch);
            col += delta;
        }
        return col;
    }

    // Inserts a character at the specified position.
    // The position is given by counting characters from the beginning of
    // the line and is not the same as the column at which the character is
    // inserted.
    // The length of the line is fixed. So, inserting the character may
    // cause characters to fall off the end of the line and be deleted.
    // These characters are then returned.
    vector<char> insert(int position, char ch);
    vector<char> insert(int position, vector<char> chars);

    // Removes the character at the specified position.
    // The position is given by counting characters from the beginning of
    // the line and is not the same as the column at which the character is
    // inserted.
    void remove(int position);

    // Returns the position of the character at the specified column.
    // The position is given by counting characters from the beginning of
    // the line and is not the same as the column at which the character is
    // inserted.
    int pos_for_col(int col) const;
};
