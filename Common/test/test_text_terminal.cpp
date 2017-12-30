#include "catch.hpp"

#include <common/text_terminal.hpp>
#include <common/text_display_device.hpp>

class dummy_text_display_device : public text_display_device {
public:
    void clear() override {}
    void draw_char(size_t, size_t, vgachar_t) override {}
    vgachar_t get_char(size_t, size_t) const override { return vgachar_t(); }
    vgachar_t make_char(char) const override { return vgachar_t(); }
    void set_cursor_position(size_t, size_t) override {}
    size_t get_cursor_row() const override { return 0; }
    size_t get_cursor_col() const override { return 0; }
};

TEST_CASE("Break text into lines", "[text_terminal]")
{
    dummy_text_display_device dummy_display;
    text_terminal term(dummy_display);

    term.get_buffer().insert(
        "The quick brown fox jumped over the lazy dog.\n"
        "foo\tbar\tbaz\n"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        );

    const vector<text_line> lines = term.get_lines();

    REQUIRE(lines.size() == 4);

    const char *ex0 = "The quick brown fox jumped over the lazy dog.";
    INFO(ex0);
    REQUIRE(lines[0].get() == vector<char>(strlen(ex0), ex0));

    const char *ex1 = "foo\tbar\tbaz";
    INFO(ex1);
    REQUIRE(lines[1].get() == vector<char>(strlen(ex1), ex1));

    const char *ex2 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    INFO(ex2);
    REQUIRE(lines[2].get() == vector<char>(strlen(ex2), ex2));

    const char *ex3 = "aaaaa";
    INFO(ex3);
    REQUIRE(lines[3].get() == vector<char>(strlen(ex3), ex3));
}
