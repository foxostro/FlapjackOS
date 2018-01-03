#include "catch.hpp"

#include <common/line_editor.hpp>
#include <common/text_terminal.hpp>
#include "dummy_text_display_device.hpp"
#include "dummy_keyboard_device.hpp"
#include <vector>

TEST_CASE("line_editor basic input", "[line_editor]")
{
    const char *s = "hello";
    vector<char> expected(strlen(s), s);

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

    vector<char> line = ed.getline();

    line.push_back('\0');
    expected.push_back('\0');
    REQUIRE(std::string(line.data()) == std::string(expected.data()));
}

TEST_CASE("line_editor backspace at the end", "[line_editor]")
{
    const char *s = "f";
    vector<char> expected(strlen(s), s);

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

    vector<char> line = ed.getline();

    line.push_back('\0');
    expected.push_back('\0');
    REQUIRE(std::string(line.data()) == std::string(expected.data()));
}

TEST_CASE("line_editor left and right arrows", "[line_editor]")
{
    const char *s = "hello";
    vector<char> expected(strlen(s), s);

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

    vector<char> line = ed.getline();

    line.push_back('\0');
    expected.push_back('\0');
    REQUIRE(std::string(line.data()) == std::string(expected.data()));
}
