#include "catch.hpp"

#include <common/text_line.hpp>
#include "dummy_text_display_device.hpp"

TEST_CASE("TextLine::measure, empty line", "[TextLine]")
{
    DummyTextDisplayDevice display;
    TextLine line(display);
    Size2 phys_size = line.measure();
    REQUIRE(phys_size.width == 0);
    REQUIRE(phys_size.height == 1);
}

TEST_CASE("TextLine::push_back", "[TextLine]")
{
    DummyTextDisplayDevice display;

    constexpr Vector<char>::size_type WIDTH = CONSOLE_WIDTH;

    Vector<char> expected;
    for (Vector<char>::size_type i = 0; i < WIDTH; ++i) {
        expected.push_back('a');
    }
    expected.push_back(0);

    TextLine line(display);

    for (Vector<char>::size_type i = 0; i < WIDTH; ++i) {
        REQUIRE(line.push_back('a') == true);
    }

    // We can't fit any more characters into the line.
    REQUIRE(line.push_back('a') == false);
    
    // Make sure the line contains the data we expect.
    REQUIRE(std::string(line.str().data()) == std::string(expected.data()));
    REQUIRE(line.measure().width == display.dimensions().width);
}

TEST_CASE("TextLine::pop_back", "[TextLine]")
{
    Size2 phys_size;
    DummyTextDisplayDevice display;
    TextLine line(display);

    REQUIRE(line.push_back('H') == true);
    REQUIRE(line.push_back('e') == true);
    REQUIRE(line.push_back('l') == true);
    REQUIRE(line.push_back('l') == true);
    REQUIRE(line.push_back('o') == true);
    REQUIRE(line.push_back('!') == true);

    phys_size = line.measure();
    REQUIRE(phys_size.width == strlen("Hello!"));
    REQUIRE(phys_size.height == 1);

    REQUIRE(std::string(line.str().data()) == "Hello!");
    line.pop_back();
    REQUIRE(std::string(line.str().data()) == "Hello");

    // The logical line spans several display lines.
    phys_size = line.measure();
    REQUIRE(phys_size.width == strlen("Hello"));
    REQUIRE(phys_size.height == 1);
}

TEST_CASE("TextLine::pop_back, empty line", "[TextLine]")
{
    DummyTextDisplayDevice display;
    TextLine line(display);
    REQUIRE(std::string(line.str().data()) == "");
    line.pop_back();
    REQUIRE(std::string(line.str().data()) == "");
}

TEST_CASE("TextLine::insert", "[TextLine]")
{
    DummyTextDisplayDevice display;
    TextLine line(display);
    REQUIRE(std::string(line.str().data()) == "");

    line.insert(0, 'a');
    REQUIRE(std::string(line.str().data()) == "a");

    {
        Size2 phys_size = line.measure();
        REQUIRE(phys_size.width == 1);
        REQUIRE(phys_size.height == 1);
    }

    line.insert(0, 'b');
    REQUIRE(std::string(line.str().data()) == "ba");

    line.insert(1, 'c');
    REQUIRE(std::string(line.str().data()) == "bca");

    {
        Size2 phys_size = line.measure();
        REQUIRE(phys_size.width == 3);
        REQUIRE(phys_size.height == 1);
    }
}

TEST_CASE("TextLine::remove", "[TextLine]")
{
    DummyTextDisplayDevice display;
    TextLine line(display);

    line.push_back('a');
    line.push_back('b');
    line.push_back('c');
    line.push_back('d');
    REQUIRE(std::string(line.str().data()) == "abcd");

    {
        Size2 phys_size = line.measure();
        REQUIRE(phys_size.width == 4);
        REQUIRE(phys_size.height == 1);
    }

    line.remove(0);
    REQUIRE(std::string(line.str().data()) == "bcd");

    line.remove(1);
    REQUIRE(std::string(line.str().data()) == "bd");

    line.remove(0);
    REQUIRE(std::string(line.str().data()) == "d");

    {
        Size2 phys_size = line.measure();
        REQUIRE(phys_size.width == 1);
        REQUIRE(phys_size.height == 1);
    }

    line.remove(0);
    REQUIRE(std::string(line.str().data()) == "");

    line.remove(0);
    REQUIRE(std::string(line.str().data()) == "");
}
