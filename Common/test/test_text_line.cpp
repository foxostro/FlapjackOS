#include "catch.hpp"

#include <common/text_line.hpp>
#include <common/text_buffer.hpp>
#include <cstdio>

TEST_CASE("Insert individual characters", "[text_line]")
{
    constexpr int WIDTH = 80;
    vector<char> expected(WIDTH);
    for (int i = 0; i < WIDTH; ++i) {
        expected[i] = 'a';
    }    

    text_line line(WIDTH, 8);

    for (int i = 0; i < WIDTH; ++i) {
        REQUIRE(line.push_back('a') == true);
    }

    REQUIRE(line.push_back('a') == false);

    REQUIRE(line.columns() == WIDTH);
    REQUIRE(line.get() == expected);
}
