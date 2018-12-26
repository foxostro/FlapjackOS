#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_TEXT_LINE_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_TEXT_LINE_HPP

#include <common/text_display_device.hpp>
#include <common/ring_buffer.hpp>
#include <common/vector.hpp>
#include <common/terminal_metrics.hpp>
#include <common/vec2.hpp>

// A logical line of text in the terminal. This is a sequence of characters.
// In the terminal, these are separated by line breaks. Though, each logical
// line may flow across several physical display lines if they're long enough.
class TextLine {
    // The logical line is bound to a physical display.
    TextDisplayDevice *display_;

    using CharacterData = RingBuffer<char, CONSOLE_WIDTH>;

    // The characters in the logical line.
    CharacterData data_;

    // When we change the contents of the text line, we measure the number of
    // physical lines and rows needed to display the logical line.
    Size2 cached_display_size_;

public:
    using size_type = CharacterData::size_type;

    // The number of columns this character will take up.
    static int step_for_char(int tab_width, int col, char ch);

    // Destructor.
    ~TextLine();

    // Constructor.
    TextLine(TextDisplayDevice &display);

    // Copy constructor.
    TextLine(const TextLine &line);

    // Move constructor.
    TextLine(TextLine &&line);

    // Copy-assignment operator.
    TextLine& operator=(const TextLine &other);

    // Move-assignment operator.
    TextLine& operator=(TextLine &&other);

    // Draws the text line at the specified line of the display.
    // Returns the display line where the next logical line should be placed.
    int draw(int row);

    // Count the number of columns and rows on the display needed to draw the
    // text line.
    Size2 measure() const;

    // Convert the specified logical position to a physical position, relative
    // to this row. We do not specify the logical row, because we're always
    // referring to this row.
    Point2 convert(int logi_col) const;

    // Gets the characters in the text line.
    // The string is nul-terminated.
    Vector<char> str() const;

    inline char at(size_t index) const
    {
        return data_.at(index);
    }

    // Inserts a character at the end of the line.
    // If the character won't fit on the line then the line remains unmodified
    // and this call returns false.
    // The character must not be a newline, '\n'.
    // Returns true if this was successful, and false otherwise.
    // This may fail if there is not enough space.
    bool push_back(char ch);

    // Pops an character off the back of the line.
    // If the line is empty then this does nothing.
    void pop_back();

    // Inserts a character at the specified index of the line.
    // Returns true if this was successful, and false otherwise.
    // This may fail if there is not enough space.
    bool insert(size_type index, char ch);

    // Removes the index'th character of the line.
    void remove(size_type index);

    // Gets the number of characters in the text line.
    size_type size() const;

    // Indicates line needs to be redrawn.
    // TODO: put some methods around `dirty'
    bool dirty;

    // When we change the contents of the text line, we measure the number of
    // physical lines and rows needed to display the logical line.
    Size2 get_cached_display_size() const;

    TextDisplayDevice& get_display()
    {
        assert(display_);
        return *display_;
    }

    const TextDisplayDevice& get_display() const
    {
        assert(display_);
        return *display_;
    }
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_TEXT_LINE_HPP
