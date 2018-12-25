#include <common/text_terminal.hpp>

TextTerminal::~TextTerminal() = default;

TextTerminal::TextTerminal()
 : display_(nullptr),
   cursor_{0, 0}
{}

void TextTerminal::init(TextDisplayDevice *display)
{
    assert(display);
    display_ = display;
}

void TextTerminal::putchar(char ch)
{
    auto character = display_->make_char(ch);
    display_->draw_char(cursor_, character);
    cursor_.x++;
    if (cursor_.x >= display_->dimensions().width) {
        cursor_.x = 0;
        cursor_.y++;
    }
}

void TextTerminal::puts(const char *s)
{
    while (*s) {
        putchar(*s++);
    }
}

int TextTerminal::printf([[maybe_unused]] const char *fmt, ...)
{
    return 0;
}

void TextTerminal::move_cursor_left() {}

void TextTerminal::move_cursor_right() {}

void TextTerminal::move_cursor_to_end() {}
