#include <common/text_terminal.hpp>

TextTerminal::~TextTerminal() = default;

TextTerminal::TextTerminal()
 : display_(nullptr)
{}

void TextTerminal::init(TextDisplayDevice *display)
{
    assert(display);
    display_ = display;
}

void TextTerminal::putchar([[maybe_unused]] char ch)
{
    auto character = display_->make_char(ch);
    display_->draw_char(Point2{0, 0}, character);
}

void TextTerminal::puts([[maybe_unused]] const char *s) {}

int TextTerminal::printf([[maybe_unused]] const char *fmt, ...)
{
    return 0;
}

void TextTerminal::move_cursor_left() {}

void TextTerminal::move_cursor_right() {}

void TextTerminal::move_cursor_to_end() {}
