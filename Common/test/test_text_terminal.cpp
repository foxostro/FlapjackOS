#include "catch.hpp"

#include <common/text_terminal.hpp>
#include <common/text_display_device.hpp>

class dummy_text_display_device : public text_display_device {
    vgachar_t _buffer[CONSOLE_WIDTH*CONSOLE_HEIGHT];
    size_t _row, _col;

public:
    dummy_text_display_device()
    {
        clear();
    }

    void clear() override
    {
        memset(_buffer, 0, sizeof(_buffer));
    }

    void draw_char(size_t row, size_t col, vgachar_t ch) override
    {
        if (row < CONSOLE_HEIGHT && col < CONSOLE_WIDTH) {
            size_t i = col + CONSOLE_WIDTH * row;
            assert(i < CONSOLE_WIDTH * CONSOLE_HEIGHT);
            _buffer[i] = ch;
        }
    }

    vgachar_t get_char(size_t row, size_t col) const override
    {
        size_t i = col + CONSOLE_WIDTH * row;
        assert(i < CONSOLE_WIDTH * CONSOLE_HEIGHT);
        return _buffer[i];
    }

    vgachar_t make_char(char ch) const override
    {
        vgachar_t r;
        r.blink = 0;
        r.fg = WHITE;
        r.bg = BLACK;
        r.ch = ch;
        return r;
    }

    void set_cursor_position(size_t row, size_t col) override
    {
        _row = row;
        _col = col;
    }

    size_t get_cursor_row() const override { return _row; }
    size_t get_cursor_col() const override { return _col; }

    std::string get_line(size_t row)
    {
        std::string str;
        for (size_t col = 0; col < CONSOLE_WIDTH; ++col) {
            str += get_char(row, col).ch;
        }
        return str;
    }
};

TEST_CASE("text_terminal::puts", "[text_terminal]")
{
    dummy_text_display_device dummy_display;
    text_terminal term(dummy_display);

    term.puts(
        // Simple string
        "The quick brown fox jumped over the lazy dog.\n"

        // String with some tabs
        "foo\tbar\tbaz\n"

        // String that overflows the display line with regular characters
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcdaaa\n"

        // String that overflows the display line with a tab character
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\tc"
        );

    // Make sure logical lines look correct.
    {
        const auto &lines = term.get_logical_lines();

        const char *ex0 = "The quick brown fox jumped over the lazy dog.";
        const char *ex1 = "foo\tbar\tbaz";
        const char *ex2 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcdaaa";
        const char *ex3 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\tc";

        REQUIRE(lines.size() == 4);
        REQUIRE(std::string(lines[0].str().data()) == ex0);
        REQUIRE(std::string(lines[1].str().data()) == ex1);
        REQUIRE(std::string(lines[2].str().data()) == ex2);
        REQUIRE(std::string(lines[3].str().data()) == ex3);
    }

    // Make sure physical lines look correct.
    {
        const char *ex0 = "The quick brown fox jumped over the lazy dog.                                   ";
        const char *ex1 = "foo     bar     baz                                                             ";
        const char *ex2 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab";
        const char *ex3 = "cdaaa                                                                           ";
        const char *ex4 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa    ";
        const char *ex5 = "c                                                                               ";

        REQUIRE(dummy_display.get_line(0) == ex0);
        REQUIRE(dummy_display.get_line(1) == ex1);
        REQUIRE(dummy_display.get_line(2) == ex2);
        REQUIRE(dummy_display.get_line(3) == ex3);
        REQUIRE(dummy_display.get_line(4) == ex4);
        REQUIRE(dummy_display.get_line(5) == ex5);
    }
}
