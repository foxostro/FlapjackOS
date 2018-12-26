#include "catch.hpp"

#include <common/text_terminal.hpp>
#include "dummy_text_display_device.hpp"

TEST_CASE("putchar prints a single character", "[TextTerminal]")
{
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);

    term.putchar('a');

    REQUIRE("a" == dummy_display.get_line(0));
}

TEST_CASE("putchar advances the cursor", "[TextTerminal]")
{
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);

    for (size_t i = 0; i < CONSOLE_WIDTH; ++i) {
        term.putchar('a');
    }

    REQUIRE(std::string(CONSOLE_WIDTH, 'a') == dummy_display.get_line(0));
}

TEST_CASE("putchar flows to the next line too", "[TextTerminal]")
{
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);

    for (size_t i = 0; i < CONSOLE_WIDTH; ++i) {
        term.putchar('a');
    }
    term.putchar('b');

    REQUIRE(std::string(CONSOLE_WIDTH, 'a') == dummy_display.get_line(0));
    REQUIRE("b" == dummy_display.get_line(1));
}

TEST_CASE("puts() prints a string of characters", "[TextTerminal]")
{
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);

    term.puts("Hello, world!");

    REQUIRE("Hello, world!" == dummy_display.get_line(0));
}

TEST_CASE("puts() prints nothing when passed nullptr", "[TextTerminal]")
{
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);

    term.puts(nullptr);

    REQUIRE("" == dummy_display.get_line(0));
}

TEST_CASE("printf() prints a formatted string", "[TextTerminal]")
{
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);

    term.printf("test %d", 42);

    REQUIRE("test 42" == dummy_display.get_line(0));
}

TEST_CASE("putchar('\n') advances to the next line", "[TextTerminal]")
{
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);

    term.putchar('a');
    term.putchar('\n');
    term.putchar('b');

    REQUIRE("a" == dummy_display.get_line(0));
    REQUIRE("b" == dummy_display.get_line(1));
}

TEST_CASE("putchar('\b') removes a character and moves cursor backwards", "[TextTerminal]")
{
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);

    term.putchar('a');
    term.putchar('\b');
    term.putchar('b');

    REQUIRE("b" == dummy_display.get_line(0));
}

TEST_CASE("putchar('\b') does not move back into the previous line", "[TextTerminal]")
{
    // Setup
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);

    for (size_t i = 0; i < CONSOLE_WIDTH; ++i) {
        term.putchar('a');
    }
    term.putchar('b');

    // Action
    term.putchar('\b');
    term.putchar('\b');

    // Test
    REQUIRE(std::string(CONSOLE_WIDTH, 'a') == dummy_display.get_line(0));
    REQUIRE("" == dummy_display.get_line(1));
}

TEST_CASE("putchar(0) does nothing", "[TextTerminal]")
{
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);

    term.putchar(0);
    term.putchar('a');

    REQUIRE("a" == dummy_display.get_line(0));
}

TEST_CASE("putchar() before the end of the line inserts there", "[TextTerminal]")
{
    // Setup
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);

    term.putchar('b');
    term.move_cursor_left();

    // Action
    term.putchar('a');

    // Test
    REQUIRE("ab" == dummy_display.get_line(0));
}

TEST_CASE("cannot move cursor back further than column zero", "[TextTerminal]")
{
    // Setup
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);

    term.putchar('b');
    term.move_cursor_left();
    term.move_cursor_left();
    term.move_cursor_left();

    // Action
    term.putchar('a');

    // Test
    REQUIRE("ab" == dummy_display.get_line(0));
}

TEST_CASE("can move cursor right too", "[TextTerminal]")
{
    // Setup
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);

    term.putchar('a');
    term.move_cursor_left();

    // Action
    term.move_cursor_right();
    term.putchar('b');

    // Test
    REQUIRE("ab" == dummy_display.get_line(0));
}

TEST_CASE("cannot move cursor further right than end of line contents", "[TextTerminal]")
{
    // Setup
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);

    term.putchar('b');
    term.putchar('c');
    term.move_cursor_left();
    term.move_cursor_left();
    term.putchar('a');

    // Action
    term.move_cursor_right();
    term.move_cursor_right();
    term.move_cursor_right();
    term.move_cursor_right();
    term.putchar('d');

    // Test
    REQUIRE("abcd" == dummy_display.get_line(0));
}
