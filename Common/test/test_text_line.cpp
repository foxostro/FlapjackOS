#include "catch.hpp"

#include <common/text_line.hpp>
#include <common/terminal_metrics.hpp>
#include <string>

TEST_CASE("Count columns, regular characters", "[text_line]")
{
    text_buffer text;
    text.insert("The quick brown fox jumped over the lazy dog.");

    text_line line(&text, TAB_WIDTH);
    line.begin.set(0);
    line.end.set(strlen("The quick brown fox jumped over the lazy dog.")-1);

    REQUIRE(line.columns() == 45);
}

TEST_CASE("Count columns, include tabs", "[text_line]")
{
    text_buffer text;
    text.insert("\ta\tb");

    text_line line(&text, TAB_WIDTH);
    line.begin.set(0);
    line.end.set(strlen("\ta\tb")-1);

    REQUIRE(line.columns() == 17);
}

TEST_CASE("Get line data", "[text_line]")
{
    text_buffer text;
    text.insert("The quick brown fox jumped over the lazy dog.");

    text_line line(&text, TAB_WIDTH);
    line.begin.set(0);
    line.end.set(strlen("The quick brown fox jumped over the lazy dog.")-1);

    REQUIRE(std::string(line.get().data()) == "The quick brown fox jumped over the lazy dog.");
}
