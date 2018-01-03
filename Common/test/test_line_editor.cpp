#include "catch.hpp"

#include <common/line_editor.hpp>
#include <common/text_terminal.hpp>
#include "dummy_text_display_device.hpp"
#include "dummy_keyboard_device.hpp"
#include <vector>
#include <string>

using line_t = line_editor::line_t;

static line_t build_line(const std::string &str)
{
    line_t line;
    for (int i = 0, n = str.size(); i < n; ++i) {
        line.push_back(str[i]);
    }
    return line;
}

static std::string to_string(const line_t &line)
{
    std::string result;
    for (int i = 0, n = line.size(); i < n; ++i) {
        result += line[i];
    }
    return result;
}

TEST_CASE("line_editor basic input", "[line_editor]")
{
    line_t expected = build_line("hello");

    dummy_text_display_device display;
    dummy_keyboard_device keyboard;
    text_terminal term(display);

    keyboard.set_events(std::vector<keyboard_event>{
        keyboard_event(KEYCODE_H, PRESSED, 'h'),
        keyboard_event(KEYCODE_H, RELEASED, 'h'),
        keyboard_event(KEYCODE_E, PRESSED, 'e'),
        keyboard_event(KEYCODE_E, RELEASED, 'e'),
        keyboard_event(KEYCODE_L, PRESSED, 'l'),
        keyboard_event(KEYCODE_L, RELEASED, 'l'),
        keyboard_event(KEYCODE_L, PRESSED, 'l'),
        keyboard_event(KEYCODE_L, RELEASED, 'l'),
        keyboard_event(KEYCODE_O, PRESSED, 'o'),
        keyboard_event(KEYCODE_O, RELEASED, 'o'),
        keyboard_event(KEYCODE_ENTER, PRESSED, '\n'),
        keyboard_event(KEYCODE_ENTER, RELEASED, '\n')
    });

    line_editor ed(term, keyboard);

    line_t line = ed.getline();

    REQUIRE(line == expected);
    REQUIRE(display.get_line( 0) == "> hello                                                                         ");
}

TEST_CASE("line_editor backspace at the end", "[line_editor]")
{
    dummy_text_display_device display;
    dummy_keyboard_device keyboard;
    text_terminal term(display);

    keyboard.set_events(std::vector<keyboard_event>{
        keyboard_event(KEYCODE_H, PRESSED, 'h'),
        keyboard_event(KEYCODE_H, RELEASED, 'h'),
        keyboard_event(KEYCODE_E, PRESSED, 'e'),
        keyboard_event(KEYCODE_E, RELEASED, 'e'),
        keyboard_event(KEYCODE_L, PRESSED, 'l'),
        keyboard_event(KEYCODE_L, RELEASED, 'l'),
        keyboard_event(KEYCODE_L, PRESSED, 'l'),
        keyboard_event(KEYCODE_L, RELEASED, 'l'),
        keyboard_event(KEYCODE_O, PRESSED, 'o'),
        keyboard_event(KEYCODE_O, RELEASED, 'o'),

        keyboard_event(KEYCODE_BACKSPACE, PRESSED, '\b'), // o
        keyboard_event(KEYCODE_BACKSPACE, RELEASED, '\b'),
        keyboard_event(KEYCODE_BACKSPACE, PRESSED, '\b'), // l
        keyboard_event(KEYCODE_BACKSPACE, RELEASED, '\b'),
        keyboard_event(KEYCODE_BACKSPACE, PRESSED, '\b'), // l
        keyboard_event(KEYCODE_BACKSPACE, RELEASED, '\b'),
        keyboard_event(KEYCODE_BACKSPACE, PRESSED, '\b'), // e
        keyboard_event(KEYCODE_BACKSPACE, RELEASED, '\b'),
        keyboard_event(KEYCODE_BACKSPACE, PRESSED, '\b'), // h
        keyboard_event(KEYCODE_BACKSPACE, RELEASED, '\b'),
        keyboard_event(KEYCODE_BACKSPACE, PRESSED, '\b'), // does nothing in the editor
        keyboard_event(KEYCODE_BACKSPACE, RELEASED, '\b'),

        keyboard_event(KEYCODE_O, PRESSED, 'f'),
        keyboard_event(KEYCODE_O, RELEASED, 'f'),

        keyboard_event(KEYCODE_ENTER, PRESSED, '\n'),
        keyboard_event(KEYCODE_ENTER, RELEASED, '\n')
    });

    line_editor ed(term, keyboard);

    line_t line = ed.getline();

    REQUIRE(display.get_line( 0) == "> f                                                                             ");
}

TEST_CASE("line_editor left and right arrows", "[line_editor]")
{
    line_t expected = build_line("hello");

    dummy_text_display_device display;
    dummy_keyboard_device keyboard;
    text_terminal term(display);

    keyboard.set_events(std::vector<keyboard_event>{
        keyboard_event(KEYCODE_H, PRESSED, 'h'),
        keyboard_event(KEYCODE_H, RELEASED, 'h'),
        keyboard_event(KEYCODE_A, PRESSED, 'a'),
        keyboard_event(KEYCODE_A, RELEASED, 'a'),
        keyboard_event(KEYCODE_L, PRESSED, 'l'),
        keyboard_event(KEYCODE_L, RELEASED, 'l'),

        keyboard_event(KEYCODE_LEFT_ARROW, PRESSED, 0),
        keyboard_event(KEYCODE_LEFT_ARROW, RELEASED, 0),

        keyboard_event(KEYCODE_BACKSPACE, PRESSED, '\b'),
        keyboard_event(KEYCODE_BACKSPACE, RELEASED, '\b'),
        keyboard_event(KEYCODE_E, PRESSED, 'e'),
        keyboard_event(KEYCODE_E, RELEASED, 'e'),

        keyboard_event(KEYCODE_RIGHT_ARROW, PRESSED, 0),
        keyboard_event(KEYCODE_RIGHT_ARROW, RELEASED, 0),

        keyboard_event(KEYCODE_L, PRESSED, 'l'),
        keyboard_event(KEYCODE_L, RELEASED, 'l'),
        keyboard_event(KEYCODE_O, PRESSED, 'o'),
        keyboard_event(KEYCODE_O, RELEASED, 'o'),

        keyboard_event(KEYCODE_ENTER, PRESSED, '\n'),
        keyboard_event(KEYCODE_ENTER, RELEASED, '\n')
    });

    line_editor ed(term, keyboard);

    line_t line = ed.getline();

    REQUIRE(line == expected);
    REQUIRE(display.get_line( 0) == "> hello                                                                         ");
}

TEST_CASE("line_editor history navigation", "[line_editor]")
{
    dummy_text_display_device display;
    dummy_keyboard_device keyboard;
    text_terminal term(display);

    keyboard.set_events(std::vector<keyboard_event>{
        keyboard_event(KEYCODE_F, PRESSED, 'f'),
        keyboard_event(KEYCODE_F, RELEASED, 'f'),
        keyboard_event(KEYCODE_O, PRESSED, 'o'),
        keyboard_event(KEYCODE_O, RELEASED, 'o'),
        keyboard_event(KEYCODE_O, PRESSED, 'o'),
        keyboard_event(KEYCODE_O, RELEASED, 'o'),
        keyboard_event(KEYCODE_ENTER, PRESSED, '\n'),
        keyboard_event(KEYCODE_ENTER, RELEASED, '\n'),
        keyboard_event(KEYCODE_B, PRESSED, 'b'),
        keyboard_event(KEYCODE_B, RELEASED, 'b'),
        keyboard_event(KEYCODE_A, PRESSED, 'a'),
        keyboard_event(KEYCODE_A, RELEASED, 'a'),
        keyboard_event(KEYCODE_R, PRESSED, 'r'),
        keyboard_event(KEYCODE_R, RELEASED, 'r'),
        keyboard_event(KEYCODE_ENTER, PRESSED, '\n'),
        keyboard_event(KEYCODE_ENTER, RELEASED, '\n'),
        keyboard_event(KEYCODE_B, PRESSED, 'b'),
        keyboard_event(KEYCODE_B, RELEASED, 'b'),
        keyboard_event(KEYCODE_A, PRESSED, 'a'),
        keyboard_event(KEYCODE_A, RELEASED, 'a'),
        keyboard_event(KEYCODE_Z, PRESSED, 'z'),
        keyboard_event(KEYCODE_Z, RELEASED, 'z'),
        keyboard_event(KEYCODE_ENTER, PRESSED, '\n'),
        keyboard_event(KEYCODE_ENTER, RELEASED, '\n'),
        keyboard_event(KEYCODE_A, PRESSED, 'a'),
        keyboard_event(KEYCODE_A, RELEASED, 'a'),
        keyboard_event(KEYCODE_S, PRESSED, 's'),
        keyboard_event(KEYCODE_S, RELEASED, 's'),
        keyboard_event(KEYCODE_D, PRESSED, 'd'),
        keyboard_event(KEYCODE_D, RELEASED, 'd'),
        keyboard_event(KEYCODE_F, PRESSED, 'f'),
        keyboard_event(KEYCODE_F, RELEASED, 'f'),
        keyboard_event(KEYCODE_UP_ARROW, PRESSED, 0),
        keyboard_event(KEYCODE_UP_ARROW, RELEASED, 0),
        keyboard_event(KEYCODE_UP_ARROW, PRESSED, 0),
        keyboard_event(KEYCODE_UP_ARROW, RELEASED, 0),
        keyboard_event(KEYCODE_ENTER, PRESSED, '\n'),
        keyboard_event(KEYCODE_ENTER, RELEASED, '\n')
    });

    line_editor ed(term, keyboard);

    line_t line;

    line = ed.getline();
    REQUIRE(to_string(line) == "foo");
    ed.add_history(line);

    line = ed.getline();
    REQUIRE(to_string(line) == "bar");
    ed.add_history(line);

    line = ed.getline();
    REQUIRE(to_string(line) == "baz");
    ed.add_history(line);

    line = ed.getline();
    REQUIRE(to_string(line) == "bar");
}

TEST_CASE("backspace after history recall", "[line_editor]")
{
    dummy_text_display_device display;
    dummy_keyboard_device keyboard;
    text_terminal term(display);

    keyboard.set_events(std::vector<keyboard_event>{
        keyboard_event(KEYCODE_F, PRESSED, 'f'),
        keyboard_event(KEYCODE_F, RELEASED, 'f'),
        keyboard_event(KEYCODE_O, PRESSED, 'o'),
        keyboard_event(KEYCODE_O, RELEASED, 'o'),
        keyboard_event(KEYCODE_O, PRESSED, 'o'),
        keyboard_event(KEYCODE_O, RELEASED, 'o'),
        keyboard_event(KEYCODE_ENTER, PRESSED, '\n'),
        keyboard_event(KEYCODE_ENTER, RELEASED, '\n'),
        keyboard_event(KEYCODE_UP_ARROW, PRESSED, 0),
        keyboard_event(KEYCODE_UP_ARROW, RELEASED, 0),
        keyboard_event(KEYCODE_BACKSPACE, PRESSED, '\b'),
        keyboard_event(KEYCODE_BACKSPACE, RELEASED, '\b'),
        keyboard_event(KEYCODE_ENTER, PRESSED, '\n'),
        keyboard_event(KEYCODE_ENTER, RELEASED, '\n')
    });

    line_editor ed(term, keyboard);

    line_t line;

    line = ed.getline();
    REQUIRE(to_string(line) == "foo");
    REQUIRE(display.get_line( 0) == "> foo                                                                           ");
    ed.add_history(line);

    line = ed.getline();
    REQUIRE(to_string(line) == "fo");
    REQUIRE(display.get_line( 1) == "> fo                                                                            ");
    ed.add_history(line);
}
