#include "catch.hpp"

#include <common/text_buffer.hpp>

TEST_CASE("Insert a printable character", "[text_buffer]")
{
    text_buffer text;

    REQUIRE(text.get_cursor().as_index() == 0);
    REQUIRE(text.size() == 0);

    text.insert('a');

    REQUIRE(text.get_cursor().as_index() == 1);
    REQUIRE(text.size() == 1);
    REQUIRE(text[0] == 'a');
}

TEST_CASE("Insert tabs and newlines", "[text_buffer]")
{
    text_buffer text;
    text.insert("a\n\tb");
    REQUIRE(text.size() == 4);
}

TEST_CASE("Insert a non-printable character", "[text_buffer]")
{
    text_buffer text;

    REQUIRE(text.get_cursor().as_index() == 0);
    REQUIRE(text.size() == 0);

    text.insert('\0');

    REQUIRE(text.get_cursor().as_index() == 0);
    REQUIRE(text.size() == 0);
}

TEST_CASE("Backspace", "[text_buffer]")
{
    text_buffer text;

    REQUIRE(text.get_cursor().as_index() == 0);
    REQUIRE(text.size() == 0);

    text.insert('a');

    REQUIRE(text.get_cursor().as_index() == 1);
    REQUIRE(text.size() == 1);
    REQUIRE(text[0] == 'a');

    // Backspace removes the character we inserted.
    text.insert('\b');
    REQUIRE(text.get_cursor().as_index() == 0);
    REQUIRE(text.size() == 0);

    // Subsequent backspaces do nothing.
    text.insert('\b');
    REQUIRE(text.get_cursor().as_index() == 0);
    REQUIRE(text.size() == 0);
}

TEST_CASE("Set cursor position to beginning of buffer", "[text_buffer]")
{
    text_buffer text;

    REQUIRE(text.get_cursor().as_index() == 0);
    REQUIRE(text.size() == 0);

    text.insert("The quick brown fox jumped over the lazy dog.");

    text.get_cursor().set(0);
    REQUIRE(text.get_cursor().as_index() == 0);
}

TEST_CASE("Discard", "[text_buffer]")
{
    text_buffer text;

    REQUIRE(text.get_cursor().as_index() == 0);
    REQUIRE(text.size() == 0);

    text.insert("The quick brown fox jumped over the lazy dog.");

    text.get_cursor().move_backward();
    text.discard(text.get_cursor());

    REQUIRE(text.get_cursor().as_index() == 0);
    REQUIRE(text.size() == 1);
}
