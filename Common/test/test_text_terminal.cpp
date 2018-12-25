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
