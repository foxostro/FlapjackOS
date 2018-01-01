#include "catch.hpp"

#include <common/text_terminal.hpp>
#include "dummy_text_display_device.hpp"

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

TEST_CASE("logical lines are limited", "[text_terminal]")
{
    dummy_text_display_device dummy_display;
    text_terminal term(dummy_display);

    // When we put more logical lines than can fit on the display, the top line
    // should be discarded to make room.
    term.puts(
        "i=0\n"
        "i=1\n"
        "i=2\n"
        "i=3\n"
        "i=4\n"
        "i=5\n"
        "i=6\n"
        "i=7\n"
        "i=8\n"
        "i=9\n"
        "i=10\n"
        "i=11\n"
        "i=12\n"
        "i=13\n"
        "i=14\n"
        "i=15\n"
        "i=16\n"
        "i=17\n"
        "i=18\n"
        "i=19\n"
        "i=20\n"
        "i=21\n"
        "i=22\n"
        "i=23\n"
        "i=24\n"
        "i=25"
        );

    // Make sure logical lines look correct.
    {
        const auto &lines = term.get_logical_lines();
        REQUIRE(lines.size() == CONSOLE_HEIGHT);
        REQUIRE(std::string(lines[0].str().data()) == "i=1");
        REQUIRE(std::string(lines[CONSOLE_HEIGHT-1].str().data()) == "i=25");
    }

    // Make sure physical lines look correct.
    REQUIRE(dummy_display.get_line( 0) == "i=1                                                                             ");
    REQUIRE(dummy_display.get_line( 1) == "i=2                                                                             ");
    REQUIRE(dummy_display.get_line( 2) == "i=3                                                                             ");
    REQUIRE(dummy_display.get_line( 3) == "i=4                                                                             ");
    REQUIRE(dummy_display.get_line( 4) == "i=5                                                                             ");
    REQUIRE(dummy_display.get_line( 5) == "i=6                                                                             ");
    REQUIRE(dummy_display.get_line( 6) == "i=7                                                                             ");
    REQUIRE(dummy_display.get_line( 7) == "i=8                                                                             ");
    REQUIRE(dummy_display.get_line( 8) == "i=9                                                                             ");
    REQUIRE(dummy_display.get_line( 9) == "i=10                                                                            ");
    REQUIRE(dummy_display.get_line(10) == "i=11                                                                            ");
    REQUIRE(dummy_display.get_line(11) == "i=12                                                                            ");
    REQUIRE(dummy_display.get_line(12) == "i=13                                                                            ");
    REQUIRE(dummy_display.get_line(13) == "i=14                                                                            ");
    REQUIRE(dummy_display.get_line(14) == "i=15                                                                            ");
    REQUIRE(dummy_display.get_line(15) == "i=16                                                                            ");
    REQUIRE(dummy_display.get_line(16) == "i=17                                                                            ");
    REQUIRE(dummy_display.get_line(17) == "i=18                                                                            ");
    REQUIRE(dummy_display.get_line(18) == "i=19                                                                            ");
    REQUIRE(dummy_display.get_line(19) == "i=20                                                                            ");
    REQUIRE(dummy_display.get_line(20) == "i=21                                                                            ");
    REQUIRE(dummy_display.get_line(21) == "i=22                                                                            ");
    REQUIRE(dummy_display.get_line(22) == "i=23                                                                            ");
    REQUIRE(dummy_display.get_line(23) == "i=24                                                                            ");
    REQUIRE(dummy_display.get_line(24) == "i=25                                                                            ");
}

TEST_CASE("scrolling when logical lines overflow physical lines", "[text_terminal]")
{
    dummy_text_display_device dummy_display;
    text_terminal term(dummy_display);

    term.puts(
        "i=0\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=1\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=2\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=3\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=4\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=5\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=6\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=7\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=8\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=9\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=10\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=11\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=12\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=13\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=14\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=15\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=16\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=17\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=18\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=19\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=20\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=21\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=22\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=23\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=24\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab\n"
        "i=25\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab"
        );

    // Make sure logical lines look correct.
    {
        const auto &lines = term.get_logical_lines();
        REQUIRE(lines.size() == CONSOLE_HEIGHT);
        REQUIRE(std::string(lines[0].str().data()) == "i=1\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab");
        REQUIRE(std::string(lines[CONSOLE_HEIGHT-1].str().data()) == "i=25\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab");
    }

    // Make sure physical lines look correct.
    REQUIRE(dummy_display.get_line( 0) == "aaaaaaab                                                                        ");
    REQUIRE(dummy_display.get_line( 1) == "i=14    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    REQUIRE(dummy_display.get_line( 2) == "aaaaaaab                                                                        ");
    REQUIRE(dummy_display.get_line( 3) == "i=15    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    REQUIRE(dummy_display.get_line( 4) == "aaaaaaab                                                                        ");
    REQUIRE(dummy_display.get_line( 5) == "i=16    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    REQUIRE(dummy_display.get_line( 6) == "aaaaaaab                                                                        ");
    REQUIRE(dummy_display.get_line( 7) == "i=17    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    REQUIRE(dummy_display.get_line( 8) == "aaaaaaab                                                                        ");
    REQUIRE(dummy_display.get_line( 9) == "i=18    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    REQUIRE(dummy_display.get_line(10) == "aaaaaaab                                                                        ");
    REQUIRE(dummy_display.get_line(11) == "i=19    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    REQUIRE(dummy_display.get_line(12) == "aaaaaaab                                                                        ");
    REQUIRE(dummy_display.get_line(13) == "i=20    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    REQUIRE(dummy_display.get_line(14) == "aaaaaaab                                                                        ");
    REQUIRE(dummy_display.get_line(15) == "i=21    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    REQUIRE(dummy_display.get_line(16) == "aaaaaaab                                                                        ");
    REQUIRE(dummy_display.get_line(17) == "i=22    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    REQUIRE(dummy_display.get_line(18) == "aaaaaaab                                                                        ");
    REQUIRE(dummy_display.get_line(19) == "i=23    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    REQUIRE(dummy_display.get_line(20) == "aaaaaaab                                                                        ");
    REQUIRE(dummy_display.get_line(21) == "i=24    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    REQUIRE(dummy_display.get_line(22) == "aaaaaaab                                                                        ");
    REQUIRE(dummy_display.get_line(23) == "i=25    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    REQUIRE(dummy_display.get_line(24) == "aaaaaaab                                                                        ");

    // And make sure the cursor position has been correctly translated from a
    // logical cursor position to a physical cursor position.
    REQUIRE(dummy_display.get_cursor_position().x == 8);
    REQUIRE(dummy_display.get_cursor_position().y == CONSOLE_HEIGHT-1);
}

TEST_CASE("putchar backspace", "[text_terminal]")
{
    dummy_text_display_device dummy_display;
    text_terminal term(dummy_display);

    term.puts("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaba");

    REQUIRE(dummy_display.get_line( 0) == "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab");
    REQUIRE(dummy_display.get_line( 1) == "a                                                                               ");
    REQUIRE(dummy_display.get_line( 2) == "                                                                                ");
    REQUIRE(dummy_display.get_cursor_position().x == 1);
    REQUIRE(dummy_display.get_cursor_position().y == 1);

    term.putchar('\b');

    REQUIRE(dummy_display.get_line( 0) == "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab");
    REQUIRE(dummy_display.get_line( 1) == "                                                                                ");
    REQUIRE(dummy_display.get_line( 2) == "                                                                                ");
    REQUIRE(dummy_display.get_cursor_position().x == 0);
    REQUIRE(dummy_display.get_cursor_position().y == 1);

    term.putchar('\b');

    REQUIRE(dummy_display.get_line( 0) == "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa ");
    REQUIRE(dummy_display.get_line( 1) == "                                                                                ");
    REQUIRE(dummy_display.get_line( 2) == "                                                                                ");
    REQUIRE(dummy_display.get_cursor_position().x == CONSOLE_WIDTH-1);
    REQUIRE(dummy_display.get_cursor_position().y == 0);
}
