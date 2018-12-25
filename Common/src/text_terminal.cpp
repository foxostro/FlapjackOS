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
    } else if (ch == '\b') {
        display_->draw_char(cursor_, display_->make_char(0));
        advance_cursor_backward();
    } else {
        display_->draw_char(cursor_, display_->make_char(ch));
        advance_cursor_forward();
    }
}

void TextTerminal::move_cursor_for_newline()
{
    cursor_.x = 0;
    cursor_.y++;
}

void TextTerminal::advance_cursor_forward()
{
    cursor_.x++;
    if (is_cursor_past_max_width()) {
        move_cursor_for_newline();
    }
}

void TextTerminal::advance_cursor_backward()
{
    if (cursor_.x > 0) {
        cursor_.x--;
    }
}

bool TextTerminal::is_cursor_past_max_width()
{
    return cursor_.x >= display_->dimensions().width;
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
