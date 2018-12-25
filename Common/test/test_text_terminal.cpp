#include "catch.hpp"

#include <common/text_terminal.hpp>
#include "dummy_text_display_device.hpp"

TEST_CASE("TextTerminal::putchar('a')", "[TextTerminal]")
{
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);

    term.putchar('a');

    REQUIRE("a" == dummy_display.get_line(0));
}
