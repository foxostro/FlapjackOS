#include "catch.hpp"

#include <common/text_line.hpp>
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
        auto overflow = line.insert(0, 'a');
        REQUIRE(overflow.empty());
    }

    REQUIRE(line.columns() == WIDTH);
    REQUIRE(line.data() == expected);
}

TEST_CASE("Insert tab to cause overflow", "[text_line]")
{
    constexpr int WIDTH = 16;

    const char *expected1 = "aaaaaaaaaaaaaaaa";
    const char *expected2 = "\taaaaaaaa";
    const char *expected_overflow = "aaaaaaaa";

    const vector<char> buf(strlen(expected1), expected1);
    text_line line(WIDTH, 8);
    line.insert(0, buf);
    REQUIRE(line.data() == buf);
    REQUIRE(line.columns() == WIDTH);

    const auto overflow = line.insert(0, '\t');

    REQUIRE(line.data() == vector<char>(strlen(expected2), expected2));
    REQUIRE(line.columns() == WIDTH);
    REQUIRE(overflow == vector<char>(strlen(expected_overflow), expected_overflow));
}

TEST_CASE("Insert characters to cause overflow", "[text_line]")
{
    constexpr int WIDTH = 16;

    const char *expected1 = "aaaaaaaaaaaaaaaa";
    const char *to_insert = "bbbbbbbb";
    const char *expected2 = "bbbbbbbbaaaaaaaa";
    const char *expected_overflow = "aaaaaaaa";

    const vector<char> buf(strlen(expected1), expected1);
    text_line line(WIDTH, 8);
    line.insert(0, buf);
    REQUIRE(line.data() == buf);
    REQUIRE(line.columns() == WIDTH);

    const auto overflow = line.insert(0, vector<char>(strlen(to_insert), to_insert));

    REQUIRE(line.data() == vector<char>(strlen(expected2), expected2));
    REQUIRE(line.columns() == WIDTH);
    REQUIRE(overflow == vector<char>(strlen(expected_overflow), expected_overflow));
}

TEST_CASE("Get position from column", "[text_line]")
{
    const char *str = "\taaaaaaaa";
    const vector<char> buf(strlen(str), str);
    const text_line line(16, 8);

    INFO("foo\n");
    int c = 0;
    line.for_each_col([&](int column, int width, char ch){
        INFO("column=" << column << "\twidth=" << width << "\tch=" << ch << "\n");
        c++;
    });
    INFO("c = " << c << "\n");

    REQUIRE(line.pos_for_col(0) == 0);
    REQUIRE(line.pos_for_col(1) == 0);
    REQUIRE(line.pos_for_col(2) == 0);
    REQUIRE(line.pos_for_col(3) == 0);
    REQUIRE(line.pos_for_col(4) == 0);
    REQUIRE(line.pos_for_col(5) == 0);
    REQUIRE(line.pos_for_col(6) == 0);
    REQUIRE(line.pos_for_col(7) == 0);
    REQUIRE(line.pos_for_col(8) == 1);
    REQUIRE(line.pos_for_col(9) == 2);
}
