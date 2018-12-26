#include "catch.hpp"

#include <common/line_editor.hpp>
#include <common/text_terminal.hpp>
#include "dummy_text_display_device.hpp"
#include "dummy_keyboard_device.hpp"
#include <vector>
#include <string>

using line_t = LineEditor::Line;

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

TEST_CASE("LineEditor basic input", "[LineEditor]")
{
    line_t expected = build_line("hello");

    DummyTextDisplayDevice display;
    DummyKeyboardDevice keyboard;
    TextTerminal term; term.init(&display);

    keyboard.set_events(std::vector<KeyboardEvent>{
        KeyboardEvent(KEYCODE_H, PRESSED, 'h'),
        KeyboardEvent(KEYCODE_H, RELEASED, 'h'),
        KeyboardEvent(KEYCODE_E, PRESSED, 'e'),
        KeyboardEvent(KEYCODE_E, RELEASED, 'e'),
        KeyboardEvent(KEYCODE_L, PRESSED, 'l'),
        KeyboardEvent(KEYCODE_L, RELEASED, 'l'),
        KeyboardEvent(KEYCODE_L, PRESSED, 'l'),
        KeyboardEvent(KEYCODE_L, RELEASED, 'l'),
        KeyboardEvent(KEYCODE_O, PRESSED, 'o'),
        KeyboardEvent(KEYCODE_O, RELEASED, 'o'),
        KeyboardEvent(KEYCODE_ENTER, PRESSED, '\n'),
        KeyboardEvent(KEYCODE_ENTER, RELEASED, '\n')
    });

    LineEditor ed(term, keyboard);

    line_t line = ed.getline();

    REQUIRE(line == expected);
    REQUIRE(display.get_line( 0) == "> hello");
}

TEST_CASE("LineEditor backspace at the end", "[LineEditor]")
{
    DummyTextDisplayDevice display;
    DummyKeyboardDevice keyboard;
    TextTerminal term; term.init(&display);

    keyboard.set_events(std::vector<KeyboardEvent>{
        KeyboardEvent(KEYCODE_H, PRESSED, 'h'),
        KeyboardEvent(KEYCODE_H, RELEASED, 'h'),
        KeyboardEvent(KEYCODE_E, PRESSED, 'e'),
        KeyboardEvent(KEYCODE_E, RELEASED, 'e'),
        KeyboardEvent(KEYCODE_L, PRESSED, 'l'),
        KeyboardEvent(KEYCODE_L, RELEASED, 'l'),
        KeyboardEvent(KEYCODE_L, PRESSED, 'l'),
        KeyboardEvent(KEYCODE_L, RELEASED, 'l'),
        KeyboardEvent(KEYCODE_O, PRESSED, 'o'),
        KeyboardEvent(KEYCODE_O, RELEASED, 'o'),

        KeyboardEvent(KEYCODE_BACKSPACE, PRESSED, '\b'), // o
        KeyboardEvent(KEYCODE_BACKSPACE, RELEASED, '\b'),
        KeyboardEvent(KEYCODE_BACKSPACE, PRESSED, '\b'), // l
        KeyboardEvent(KEYCODE_BACKSPACE, RELEASED, '\b'),
        KeyboardEvent(KEYCODE_BACKSPACE, PRESSED, '\b'), // l
        KeyboardEvent(KEYCODE_BACKSPACE, RELEASED, '\b'),
        KeyboardEvent(KEYCODE_BACKSPACE, PRESSED, '\b'), // e
        KeyboardEvent(KEYCODE_BACKSPACE, RELEASED, '\b'),
        KeyboardEvent(KEYCODE_BACKSPACE, PRESSED, '\b'), // h
        KeyboardEvent(KEYCODE_BACKSPACE, RELEASED, '\b'),
        KeyboardEvent(KEYCODE_BACKSPACE, PRESSED, '\b'), // does nothing in the editor
        KeyboardEvent(KEYCODE_BACKSPACE, RELEASED, '\b'),

        KeyboardEvent(KEYCODE_O, PRESSED, 'f'),
        KeyboardEvent(KEYCODE_O, RELEASED, 'f'),

        KeyboardEvent(KEYCODE_ENTER, PRESSED, '\n'),
        KeyboardEvent(KEYCODE_ENTER, RELEASED, '\n')
    });

    LineEditor ed(term, keyboard);

    line_t line = ed.getline();

    REQUIRE(display.get_line( 0) == "> f");
}

TEST_CASE("LineEditor left and right arrows", "[LineEditor]")
{
    line_t expected = build_line("hello");

    DummyTextDisplayDevice display;
    DummyKeyboardDevice keyboard;
    TextTerminal term; term.init(&display);

    keyboard.set_events(std::vector<KeyboardEvent>{
        KeyboardEvent(KEYCODE_H, PRESSED, 'h'),
        KeyboardEvent(KEYCODE_H, RELEASED, 'h'),
        KeyboardEvent(KEYCODE_A, PRESSED, 'a'),
        KeyboardEvent(KEYCODE_A, RELEASED, 'a'),
        KeyboardEvent(KEYCODE_L, PRESSED, 'l'),
        KeyboardEvent(KEYCODE_L, RELEASED, 'l'),

        KeyboardEvent(KEYCODE_LEFT_ARROW, PRESSED, 0),
        KeyboardEvent(KEYCODE_LEFT_ARROW, RELEASED, 0),

        KeyboardEvent(KEYCODE_BACKSPACE, PRESSED, '\b'),
        KeyboardEvent(KEYCODE_BACKSPACE, RELEASED, '\b'),
        KeyboardEvent(KEYCODE_E, PRESSED, 'e'),
        KeyboardEvent(KEYCODE_E, RELEASED, 'e'),

        KeyboardEvent(KEYCODE_RIGHT_ARROW, PRESSED, 0),
        KeyboardEvent(KEYCODE_RIGHT_ARROW, RELEASED, 0),

        KeyboardEvent(KEYCODE_L, PRESSED, 'l'),
        KeyboardEvent(KEYCODE_L, RELEASED, 'l'),
        KeyboardEvent(KEYCODE_O, PRESSED, 'o'),
        KeyboardEvent(KEYCODE_O, RELEASED, 'o'),

        KeyboardEvent(KEYCODE_ENTER, PRESSED, '\n'),
        KeyboardEvent(KEYCODE_ENTER, RELEASED, '\n')
    });

    LineEditor ed(term, keyboard);

    line_t line = ed.getline();

    REQUIRE(line == expected);
    REQUIRE(display.get_line( 0) == "> hello");
}

TEST_CASE("LineEditor history navigation", "[LineEditor]")
{
    DummyTextDisplayDevice display;
    DummyKeyboardDevice keyboard;
    TextTerminal term; term.init(&display);

    keyboard.set_events(std::vector<KeyboardEvent>{
        KeyboardEvent(KEYCODE_F, PRESSED, 'f'),
        KeyboardEvent(KEYCODE_F, RELEASED, 'f'),
        KeyboardEvent(KEYCODE_O, PRESSED, 'o'),
        KeyboardEvent(KEYCODE_O, RELEASED, 'o'),
        KeyboardEvent(KEYCODE_O, PRESSED, 'o'),
        KeyboardEvent(KEYCODE_O, RELEASED, 'o'),
        KeyboardEvent(KEYCODE_ENTER, PRESSED, '\n'),
        KeyboardEvent(KEYCODE_ENTER, RELEASED, '\n'),
        KeyboardEvent(KEYCODE_B, PRESSED, 'b'),
        KeyboardEvent(KEYCODE_B, RELEASED, 'b'),
        KeyboardEvent(KEYCODE_A, PRESSED, 'a'),
        KeyboardEvent(KEYCODE_A, RELEASED, 'a'),
        KeyboardEvent(KEYCODE_R, PRESSED, 'r'),
        KeyboardEvent(KEYCODE_R, RELEASED, 'r'),
        KeyboardEvent(KEYCODE_ENTER, PRESSED, '\n'),
        KeyboardEvent(KEYCODE_ENTER, RELEASED, '\n'),
        KeyboardEvent(KEYCODE_B, PRESSED, 'b'),
        KeyboardEvent(KEYCODE_B, RELEASED, 'b'),
        KeyboardEvent(KEYCODE_A, PRESSED, 'a'),
        KeyboardEvent(KEYCODE_A, RELEASED, 'a'),
        KeyboardEvent(KEYCODE_Z, PRESSED, 'z'),
        KeyboardEvent(KEYCODE_Z, RELEASED, 'z'),
        KeyboardEvent(KEYCODE_ENTER, PRESSED, '\n'),
        KeyboardEvent(KEYCODE_ENTER, RELEASED, '\n'),
        KeyboardEvent(KEYCODE_A, PRESSED, 'a'),
        KeyboardEvent(KEYCODE_A, RELEASED, 'a'),
        KeyboardEvent(KEYCODE_S, PRESSED, 's'),
        KeyboardEvent(KEYCODE_S, RELEASED, 's'),
        KeyboardEvent(KEYCODE_D, PRESSED, 'd'),
        KeyboardEvent(KEYCODE_D, RELEASED, 'd'),
        KeyboardEvent(KEYCODE_F, PRESSED, 'f'),
        KeyboardEvent(KEYCODE_F, RELEASED, 'f'),
        KeyboardEvent(KEYCODE_UP_ARROW, PRESSED, 0),
        KeyboardEvent(KEYCODE_UP_ARROW, RELEASED, 0),
        KeyboardEvent(KEYCODE_UP_ARROW, PRESSED, 0),
        KeyboardEvent(KEYCODE_UP_ARROW, RELEASED, 0),
        KeyboardEvent(KEYCODE_ENTER, PRESSED, '\n'),
        KeyboardEvent(KEYCODE_ENTER, RELEASED, '\n')
    });

    LineEditor ed(term, keyboard);

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

TEST_CASE("backspace after history recall", "[LineEditor]")
{
    DummyTextDisplayDevice display;
    DummyKeyboardDevice keyboard;
    TextTerminal term; term.init(&display);

    keyboard.set_events(std::vector<KeyboardEvent>{
        KeyboardEvent(KEYCODE_F, PRESSED, 'f'),
        KeyboardEvent(KEYCODE_F, RELEASED, 'f'),
        KeyboardEvent(KEYCODE_O, PRESSED, 'o'),
        KeyboardEvent(KEYCODE_O, RELEASED, 'o'),
        KeyboardEvent(KEYCODE_O, PRESSED, 'o'),
        KeyboardEvent(KEYCODE_O, RELEASED, 'o'),
        KeyboardEvent(KEYCODE_ENTER, PRESSED, '\n'),
        KeyboardEvent(KEYCODE_ENTER, RELEASED, '\n'),
        KeyboardEvent(KEYCODE_UP_ARROW, PRESSED, 0),
        KeyboardEvent(KEYCODE_UP_ARROW, RELEASED, 0),
        KeyboardEvent(KEYCODE_BACKSPACE, PRESSED, '\b'),
        KeyboardEvent(KEYCODE_BACKSPACE, RELEASED, '\b'),
        KeyboardEvent(KEYCODE_ENTER, PRESSED, '\n'),
        KeyboardEvent(KEYCODE_ENTER, RELEASED, '\n')
    });

    LineEditor ed(term, keyboard);

    line_t line;

    line = ed.getline();
    REQUIRE(to_string(line) == "foo");
    REQUIRE(display.get_line( 0) == "> foo");
    ed.add_history(line);

    line = ed.getline();
    REQUIRE(to_string(line) == "fo");
    REQUIRE(display.get_line( 1) == "> fo");
    ed.add_history(line);
}

TEST_CASE("backspace after pressing left arrow does nothing", "[LineEditor]")
{
    // This test case covers a bug fix. Pressing the left arrow and then hitting
    // backspace was causing a crash.

    DummyTextDisplayDevice display;
    DummyKeyboardDevice keyboard;
    TextTerminal term;
    term.init(&display);

    keyboard.set_events(std::vector<KeyboardEvent>{
        KeyboardEvent(KEYCODE_A, PRESSED, 'a'),
        KeyboardEvent(KEYCODE_A, RELEASED, 'a'),
        KeyboardEvent(KEYCODE_LEFT_ARROW, PRESSED, 0),
        KeyboardEvent(KEYCODE_LEFT_ARROW, RELEASED, 0),
        KeyboardEvent(KEYCODE_BACKSPACE, PRESSED, '\b'),
        KeyboardEvent(KEYCODE_BACKSPACE, RELEASED, '\b'),
        KeyboardEvent(KEYCODE_ENTER, PRESSED, '\n'),
        KeyboardEvent(KEYCODE_ENTER, RELEASED, '\n')
    });

    LineEditor ed(term, keyboard);
    (void)ed.getline();

    REQUIRE(display.get_line(0) == "> a");
}
