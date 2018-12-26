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
public:
    using CharacterData = RingBuffer<char, CONSOLE_WIDTH>;
    using size_type = CharacterData::size_type;

    // Destructor.
    ~TextLine();

    // Constructor.
    TextLine(TextDisplayDevice& display);

    // Copy constructor.
    TextLine(const TextLine& line);

    // Move constructor.
    TextLine(TextLine&& line);

    // Copy-assignment operator.
    TextLine& operator=(const TextLine& other);

    // Move-assignment operator.
    TextLine& operator=(TextLine&& other);

    // Draws the text line at the specified line of the display.
    // Returns the display line where the next logical line should be placed.
    int draw(int row);

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

private:
    TextDisplayDevice& display_;
    CharacterData data_;

    // The number of columns this character will take up.
    int step_for_char(int col, char ch);
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_TEXT_LINE_HPP
