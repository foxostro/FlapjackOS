#include <common/text_terminal.hpp>
#include <cstdarg>
#include <cstdio>

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
    if (ch == '\n') {
        move_cursor_for_newline();
    } else {
        auto character = display_->make_char(ch);
        display_->draw_char(cursor_, character);
        cursor_.x++;
        if (cursor_.x >= display_->dimensions().width) {
            move_cursor_for_newline();
        }
    }
}

void TextTerminal::move_cursor_for_newline()
{
    cursor_.x = 0;
    cursor_.y++;
}

void TextTerminal::puts(const char *s)
{
    if (s) while (*s) {
        putchar(*s++);
    }
}

int TextTerminal::printf(const char *fmt, ...)
{
    constexpr size_t BUFFER_SIZE = 256;
    char buffer[BUFFER_SIZE] = {0};

    va_list args;
    va_start(args, fmt);
    int r = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    puts(buffer);

    return r;
}

void TextTerminal::move_cursor_left() {}

void TextTerminal::move_cursor_right() {}

void TextTerminal::move_cursor_to_end() {}
