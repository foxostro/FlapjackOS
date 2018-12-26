#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_TEXT_TERMINAL_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_TEXT_TERMINAL_HPP

#include <common/text_display_device.hpp>
#include <common/ring_buffer.hpp>
#include <common/text_line.hpp>
#include <common/vec2.hpp>

// A text terminal displays lines of text on a text console display.
class TextTerminal {
public:
    ~TextTerminal();

    // Two-phase initialization.
    TextTerminal();
    void init(TextDisplayDevice *display);

    // Puts a character on the display at the current cursor position.
    // Increments the cursor to the next position. This may wrap the cursor to
    // the next line. If the cursor reaches the bottom of the console display
    // then lines of text will scroll up off the top.
    //
    // - Many non-printable characters are ignored.
    // - Tab characters ('\t') may be wider than one column.
    // - The backspace character ('\b') deletes a character from the terminal
    //   and moves the cursor to the previous position.
    void putchar(char ch);

    // Puts each character in the vector to the terminal.
    void puts(const char *str);

    // Puts each character in the container to the terminal.
    template<typename T>
    void putv(const T &buf)
    {
        for (typename T::size_type i = 0, n = buf.size(); i < n; ++i) {
            putchar(buf[i]);
        }
    }

    // Prints a formatted string to the terminal.
    int printf(const char *fmt, ...);

    // Moves the cursor to the left.
    // If the cursor is at the beginning of the line then this does nothing.
    void move_cursor_left();

    // Moves the cursor to the right.
    // If the cursor is at the end of the line then this does nothing.
    void move_cursor_right();

    // Moves the cursor all the way to the end of the line.
    void move_cursor_to_end();

    // Scrolls one line off the top of the display.
    void scroll_one_line();

private:
    TextDisplayDevice* display_;
    Point2 cursor_;
    RingBuffer<TextLine, CONSOLE_HEIGHT> lines_;

    void backspace();
    void put_normal_character(char ch);
    void insert_char(char ch);
    void redraw_current_line();
    void redraw_line(int row);
    void draw_char(const Point2 &pos, char ch);
    void move_cursor_for_newline();
    void set_display_cursor_position();
    void advance_cursor_forward();
    void advance_cursor_backward();
    bool is_cursor_past_max_width();
    int width();
    int height();
    int get_line_width();
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_TEXT_TERMINAL_HPP
