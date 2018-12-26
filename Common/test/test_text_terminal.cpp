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

TEST_CASE("putchar('\t') prints a tab character spanning multiple columns", "[TextTerminal]")
{
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);

    term.putchar('a');
    term.putchar('\t');
    term.putchar('b');

    REQUIRE("a       b" == dummy_display.get_line(0));
}

TEST_CASE("we can backspace over a tab spanning multiple columns", "[TextTerminal]")
{
    // Setup
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);

    term.putchar('a');
    term.putchar('\t');

    // Action
    term.putchar('\b');
    term.putchar('\b');

    // Test
    REQUIRE("" == dummy_display.get_line(0));
}

TEST_CASE("tab characters move us to the end of the physical console line", "[TextTerminal]")
{
    // Setup
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);

    // Action
    term.putchar('\t'); // 8
    term.putchar('\t'); // 16
    term.putchar('\t'); // 24
    term.putchar('\t'); // 32
    term.putchar('\t'); // 40
    term.putchar('\t'); // 48
    term.putchar('\t'); // 56
    term.putchar('\t'); // 64
    term.putchar('\t'); // 72
    term.putchar('\t'); // 80

    // Test
    REQUIRE(std::string(CONSOLE_WIDTH, ' ') == dummy_display.get_line(0));
}

TEST_CASE("we can overflow a physical line with fewer than CONSOLE_WIDTH characters", "[TextTerminal]")
{
    // Setup
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);

    term.putchar('\t'); // 8
    term.putchar('\t'); // 16
    term.putchar('\t'); // 24
    term.putchar('\t'); // 32
    term.putchar('\t'); // 40
    term.putchar('\t'); // 48
    term.putchar('\t'); // 56
    term.putchar('\t'); // 64
    term.putchar('\t'); // 72
    term.putchar('\t'); // 80

    // Action
    term.putchar('a');

    // Test
    REQUIRE(std::string(CONSOLE_WIDTH, ' ') == dummy_display.get_line(0));
    REQUIRE("a" == dummy_display.get_line(1));
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

TEST_CASE("move_cursor_to_end() moves to the end of the line", "[TextTerminal]")
{
    // Setup
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);

    term.putchar('a');
    term.putchar('b');
    term.move_cursor_left();
    term.move_cursor_left();

    // Action
    term.move_cursor_to_end();
    term.putchar('c');

    // Test
    REQUIRE("abc" == dummy_display.get_line(0));
}

TEST_CASE("scroll_one_line", "[TextTerminal]")
{
    // Setup
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);

    auto print_line = [&](char ch){
        for (size_t column = 0; column < CONSOLE_WIDTH; ++column) {
            term.putchar(ch);
        }
    };

    for (char ch = 'a'; ch < 'z'; ++ch) {
        print_line(ch);
    }

    // Action
    term.scroll_one_line();

    // Test
    REQUIRE(std::string(CONSOLE_WIDTH, 'b') == dummy_display.get_line(0));
    REQUIRE(std::string(CONSOLE_WIDTH, 'c') == dummy_display.get_line(1));
    REQUIRE(std::string(CONSOLE_WIDTH, 'd') == dummy_display.get_line(2));
    REQUIRE(std::string(CONSOLE_WIDTH, 'e') == dummy_display.get_line(3));
    REQUIRE(std::string(CONSOLE_WIDTH, 'f') == dummy_display.get_line(4));
    REQUIRE(std::string(CONSOLE_WIDTH, 'g') == dummy_display.get_line(5));
    REQUIRE(std::string(CONSOLE_WIDTH, 'h') == dummy_display.get_line(6));
    REQUIRE(std::string(CONSOLE_WIDTH, 'i') == dummy_display.get_line(7));
    REQUIRE(std::string(CONSOLE_WIDTH, 'j') == dummy_display.get_line(8));
    REQUIRE(std::string(CONSOLE_WIDTH, 'k') == dummy_display.get_line(9));
    REQUIRE(std::string(CONSOLE_WIDTH, 'l') == dummy_display.get_line(10));
    REQUIRE(std::string(CONSOLE_WIDTH, 'm') == dummy_display.get_line(11));
    REQUIRE(std::string(CONSOLE_WIDTH, 'n') == dummy_display.get_line(12));
    REQUIRE(std::string(CONSOLE_WIDTH, 'o') == dummy_display.get_line(13));
    REQUIRE(std::string(CONSOLE_WIDTH, 'p') == dummy_display.get_line(14));
    REQUIRE(std::string(CONSOLE_WIDTH, 'q') == dummy_display.get_line(15));
    REQUIRE(std::string(CONSOLE_WIDTH, 'r') == dummy_display.get_line(16));
    REQUIRE(std::string(CONSOLE_WIDTH, 's') == dummy_display.get_line(17));
    REQUIRE(std::string(CONSOLE_WIDTH, 't') == dummy_display.get_line(18));
    REQUIRE(std::string(CONSOLE_WIDTH, 'u') == dummy_display.get_line(19));
    REQUIRE(std::string(CONSOLE_WIDTH, 'v') == dummy_display.get_line(20));
    REQUIRE(std::string(CONSOLE_WIDTH, 'w') == dummy_display.get_line(21));
    REQUIRE(std::string(CONSOLE_WIDTH, 'x') == dummy_display.get_line(22));
    REQUIRE(std::string(CONSOLE_WIDTH, 'y') == dummy_display.get_line(23));
}

TEST_CASE("putchar at the bottom of the display scrolls a line off the top", "[TextTerminal]")
{
    // Setup
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);

    auto print_line = [&](char ch){
        for (size_t column = 0; column < CONSOLE_WIDTH; ++column) {
            term.putchar(ch);
        }
    };

    for (char ch = 'a'; ch < 'z'; ++ch) {
        print_line(ch);
    }

    // Action
    term.putchar('z');

    // Test
    REQUIRE(std::string(CONSOLE_WIDTH, 'b') == dummy_display.get_line(0));
    REQUIRE(std::string(CONSOLE_WIDTH, 'c') == dummy_display.get_line(1));
    REQUIRE(std::string(CONSOLE_WIDTH, 'd') == dummy_display.get_line(2));
    REQUIRE(std::string(CONSOLE_WIDTH, 'e') == dummy_display.get_line(3));
    REQUIRE(std::string(CONSOLE_WIDTH, 'f') == dummy_display.get_line(4));
    REQUIRE(std::string(CONSOLE_WIDTH, 'g') == dummy_display.get_line(5));
    REQUIRE(std::string(CONSOLE_WIDTH, 'h') == dummy_display.get_line(6));
    REQUIRE(std::string(CONSOLE_WIDTH, 'i') == dummy_display.get_line(7));
    REQUIRE(std::string(CONSOLE_WIDTH, 'j') == dummy_display.get_line(8));
    REQUIRE(std::string(CONSOLE_WIDTH, 'k') == dummy_display.get_line(9));
    REQUIRE(std::string(CONSOLE_WIDTH, 'l') == dummy_display.get_line(10));
    REQUIRE(std::string(CONSOLE_WIDTH, 'm') == dummy_display.get_line(11));
    REQUIRE(std::string(CONSOLE_WIDTH, 'n') == dummy_display.get_line(12));
    REQUIRE(std::string(CONSOLE_WIDTH, 'o') == dummy_display.get_line(13));
    REQUIRE(std::string(CONSOLE_WIDTH, 'p') == dummy_display.get_line(14));
    REQUIRE(std::string(CONSOLE_WIDTH, 'q') == dummy_display.get_line(15));
    REQUIRE(std::string(CONSOLE_WIDTH, 'r') == dummy_display.get_line(16));
    REQUIRE(std::string(CONSOLE_WIDTH, 's') == dummy_display.get_line(17));
    REQUIRE(std::string(CONSOLE_WIDTH, 't') == dummy_display.get_line(18));
    REQUIRE(std::string(CONSOLE_WIDTH, 'u') == dummy_display.get_line(19));
    REQUIRE(std::string(CONSOLE_WIDTH, 'v') == dummy_display.get_line(20));
    REQUIRE(std::string(CONSOLE_WIDTH, 'w') == dummy_display.get_line(21));
    REQUIRE(std::string(CONSOLE_WIDTH, 'x') == dummy_display.get_line(22));
    REQUIRE(std::string(CONSOLE_WIDTH, 'y') == dummy_display.get_line(23));
    REQUIRE("z" == dummy_display.get_line(24));
}

TEST_CASE("putchar moves the cursor one column for regular characters", "[TextTerminal]")
{
    // Setup
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);
    
    // Action
    term.putchar('a');

    // Test
    REQUIRE(1 == dummy_display.get_cursor_position().x);
}

TEST_CASE("putchar('\n') moves the cursor to the new line", "[TextTerminal]")
{
    // Setup
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);
    term.putchar('a');
    
    // Action
    term.putchar('\n');

    // Test
    REQUIRE(0 == dummy_display.get_cursor_position().x);
    REQUIRE(1 == dummy_display.get_cursor_position().y);
}

TEST_CASE("putchar('\b') does not move the cursor when at the beginning of line", "[TextTerminal]")
{
    // Setup
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);
    
    // Action
    term.putchar('\b');

    // Test
    REQUIRE(0 == dummy_display.get_cursor_position().x);
    REQUIRE(0 == dummy_display.get_cursor_position().y);
}

TEST_CASE("putchar('\b') moves the cursor back one column for regular characters", "[TextTerminal]")
{
    // Setup
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);
    term.putchar('a');
    
    // Action
    term.putchar('\b');

    // Test
    REQUIRE(0 == dummy_display.get_cursor_position().x);
    REQUIRE(0 == dummy_display.get_cursor_position().y);
}

TEST_CASE("move_cursor_left() does not move the cursor when at beginning of line", "[TextTerminal]")
{
    // Setup
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);
    
    // Action
    term.move_cursor_left();

    // Test
    REQUIRE(0 == dummy_display.get_cursor_position().x);
}

TEST_CASE("move_cursor_left() moves the cursor back one column", "[TextTerminal]")
{
    // Setup
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);
    term.putchar('a');
    term.putchar('b');

    // Action
    term.move_cursor_left();

    // Test
    REQUIRE(1 == dummy_display.get_cursor_position().x);
}

TEST_CASE("move_cursor_right() moves right one column", "[TextTerminal]")
{
    // Setup
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);
    term.putchar('a');
    term.putchar('b');
    term.move_cursor_left();
    term.move_cursor_left();

    // Action
    term.move_cursor_right();

    // Test
    REQUIRE(1 == dummy_display.get_cursor_position().x);
}

TEST_CASE("move_cursor_right() cannot move further right than the end of line content", "[TextTerminal]")
{
    // Setup
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);
    term.putchar('a');
    term.putchar('b');

    // Action
    term.move_cursor_right();

    // Test
    REQUIRE(2 == dummy_display.get_cursor_position().x);
}

TEST_CASE("move_cursor_to_end() moves to the end of the line content", "[TextTerminal]")
{
    // Setup
    DummyTextDisplayDevice dummy_display;
    dummy_display.clear();

    TextTerminal term;
    term.init(&dummy_display);
    term.putchar('a');
    term.putchar('b');
    term.move_cursor_left();
    term.move_cursor_left();

    // Action
    term.move_cursor_to_end();

    // Test
    REQUIRE(2 == dummy_display.get_cursor_position().x);
}