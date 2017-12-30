#pragma once

#include <common/vector.hpp>

class text_buffer {
public:
    using value_type = char;
    using size_type = vector<value_type>::size_type;

    // A position is measured in the number of characters from the end of the
    // buffer, and does not change when the beginning of the buffer is
    // discarded. An index is measured from the beginning of the buffer and
    // is the natural way of addressing the internal character array.
    class cursor_position {
        const text_buffer *_owner;
        vector<value_type>::size_type _position;

    public:
        // Constructor.
        // Constructs a position at the end of the buffer.
        cursor_position(const text_buffer *owner);

        // Constructor.
        // Constructs a position from the specified index.
        cursor_position(const text_buffer *owner, size_type index);

        // Moves the cursor to the next position.
        // If the cursor is already at the end of the buffer then this does
        // nothing.
        // Returns true if the cursor was modified by this operation.
        bool move_forward();

        // Moves the cursor to the previous position.
        // If the cursor is already at the beginning of the buffer then this
        // does nothing.
        // Returns true if the cursor was modified by this operation.
        bool move_backward();

        // Convert the position to an index.
        size_type as_index() const;

        // Sets the cursor to the position corresponding to the specified
        // character index.
        void set(size_type index);
    };

    ~text_buffer();
    text_buffer();

    // Get the current cursor position within the text buffer.
    cursor_position& get_cursor();

    // If the character is printable then insert it into the buffer at the
    // cursor position and then increments the cursor to the next position.
    //
    // If the character is a backspace ('\b') then the character at the cursor
    // is deleted and the cursor moves backwards to the previous position.
    //
    // Otherwise, if the character is non-printable then nothing happens.
    void insert(value_type c);

    // Insert the characters into the buffer at the cursor position and then
    // increments the cursor to the next position.
    // The string must be nul-terminated.
    void insert(const value_type *string);

    // Insert the characters into the buffer at the cursor position and then
    // increments the cursor to the next position.
    void insert(const vector<value_type> &string);

    // Gets the number of characters in the text buffer.
    size_type size() const;

    // Gets the index'th character of the text buffer.
    value_type& at(size_type i);

    // Gets the index'th character of the text buffer.
    value_type at(size_type i) const;

    // Gets the index'th character of the text buffer.
    value_type& operator[](size_type index);

    // Gets the index'th character of the text buffer.
    value_type operator[](size_type index) const;

    // Discard all characters behind the specified position.
    // This is all characters with a position greater than the one specified.
    // This is all characters with an index less than the one specified.
    void discard(const cursor_position &pos);

private:
    // Currently, the text buffer is implemented using a simple vector.
    // However, it would also be possible to swap this out for internal storage
    // implemented using another algorithm such as Ropes, a Piece Table, or a
    // Gap Buffer.
    vector<value_type> _data;

    cursor_position _cursor;
};
