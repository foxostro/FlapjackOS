#pragma once

#include <common/ring_buffer.hpp>
#include <common/vector.hpp>
#include <common/terminal_metrics.hpp>
#include <common/vec2.hpp>

class text_display_device;

// A logical line of text in the terminal. This is a sequence of characters.
// In the terminal, these are separated by line breaks. Though, each logical
// line may flow across several physical display lines if they're long enough.
class text_line {
    // The logical line is bound to a physical display.
    text_display_device *_display;

    // The characters in the logical line. This is limited to MAXLINE chars.
    ring_buffer<char, MAXLINE> _data;

    // When we change the contents of the text line, we measure the number of
    // physical lines and rows needed to display the logical line.
    size2_t _cached_display_size;

public:
    using size_type = ring_buffer<char, MAXLINE>::size_type;

    // The number of columns this character will take up.
    static int step_for_char(int tab_width, int col, char ch);

    // Destructor.
    ~text_line();

    // Constructor.
    text_line(text_display_device &display);

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
    int draw(int row);

    // Count the number of columns and rows on the display needed to draw the
    // text line.
    size2_t measure() const;

    // Convert the specified logical position to a physical position, relative
    // to this row. We do not specify the logical row, because we're always
    // referring to this row.
    point2_t convert(int logi_col) const;

    // Gets the characters in the text line.
    // The string is nul-terminated.
    vector<char> str() const;

    // Inserts a character at the end of the line.
    // If the character won't fit on the line then the line remains unmodified
    // and this call returns false.
    // The character must not be a newline, '\n'.
    bool push_back(char ch);

    // Gets the number of characters in the text line.
    size_type size() const;

    // Indicates line needs to be redrawn.
    bool dirty;

    // When we change the contents of the text line, we measure the number of
    // physical lines and rows needed to display the logical line.
    size2_t get_cached_display_size() const;

    text_display_device& get_display()
    {
        assert(_display);
        return *_display;
    }

    const text_display_device& get_display() const
    {
        assert(_display);
        return *_display;
    }
};
