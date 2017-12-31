#include "catch.hpp"

#include <common/text_line.hpp>
#include <common/text_buffer.hpp>
#include <common/terminal_metrics.hpp>
#include <cstdio>

TEST_CASE("Insert characters into text_line", "[text_line]")
{
    constexpr vector<char>::size_type WIDTH = MAXLINE;

    vector<char> expected;
    for (vector<char>::size_type i = 0; i < WIDTH; ++i) {
        expected.push_back('a');
    }
    expected.push_back(0);

    text_line line(CONSOLE_WIDTH, TAB_WIDTH);

    for (vector<char>::size_type i = 0; i < WIDTH; ++i) {
        REQUIRE(line.push_back('a') == true);
    }

    // We can't fit any more characters into the line.
    REQUIRE(line.push_back('a') == false);
    
    // Make sure the line contains the data we expect.
    REQUIRE(std::string(line.str().data()) == std::string(expected.data()));

    // The logical line spans several display lines.
    size2_t phys_size = line.measure();

    REQUIRE(phys_size.width == CONSOLE_WIDTH);
    REQUIRE(phys_size.height == 4);
}
