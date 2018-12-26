#include "catch.hpp"

#include <common/text_line.hpp>
#include "dummy_text_display_device.hpp"

TEST_CASE("TextLine::push_back", "[TextLine]")
{
    constexpr Vector<char>::size_type WIDTH = CONSOLE_WIDTH;

    Vector<char> expected;
    for (Vector<char>::size_type i = 0; i < WIDTH; ++i) {
        expected.push_back('a');
    }
    expected.push_back(0);

    TextLine line;

    for (Vector<char>::size_type i = 0; i < WIDTH; ++i) {
        REQUIRE(line.push_back('a') == true);
    }

    // We can't fit any more characters into the line.
    REQUIRE(line.push_back('a') == false);
    
    // Make sure the line contains the data we expect.
    REQUIRE(std::string(line.str().data()) == std::string(expected.data()));
}

TEST_CASE("TextLine::pop_back", "[TextLine]")
{
    TextLine line;

    REQUIRE(line.push_back('H') == true);
    REQUIRE(line.push_back('e') == true);
    REQUIRE(line.push_back('l') == true);
    REQUIRE(line.push_back('l') == true);
    REQUIRE(line.push_back('o') == true);
    REQUIRE(line.push_back('!') == true);

    REQUIRE(std::string(line.str().data()) == "Hello!");
    line.pop_back();
    REQUIRE(std::string(line.str().data()) == "Hello");
}

TEST_CASE("TextLine::pop_back, empty line", "[TextLine]")
{
    TextLine line;
    REQUIRE(std::string(line.str().data()) == "");
    line.pop_back();
    REQUIRE(std::string(line.str().data()) == "");
}

TEST_CASE("TextLine::insert", "[TextLine]")
{
    TextLine line;
    REQUIRE(std::string(line.str().data()) == "");

    line.insert(0, 'a');
    REQUIRE(std::string(line.str().data()) == "a");

    line.insert(0, 'b');
    REQUIRE(std::string(line.str().data()) == "ba");

    line.insert(1, 'c');
    REQUIRE(std::string(line.str().data()) == "bca");
}

TEST_CASE("TextLine::remove", "[TextLine]")
{
    TextLine line;

    line.push_back('a');
    line.push_back('b');
    line.push_back('c');
    line.push_back('d');
    REQUIRE(std::string(line.str().data()) == "abcd");

    line.remove(0);
    REQUIRE(std::string(line.str().data()) == "bcd");

    line.remove(1);
    REQUIRE(std::string(line.str().data()) == "bd");

    line.remove(0);
    REQUIRE(std::string(line.str().data()) == "d");

    line.remove(0);
    REQUIRE(std::string(line.str().data()) == "");

    line.remove(0);
    REQUIRE(std::string(line.str().data()) == "");
}
