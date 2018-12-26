#include <common/text_terminal.hpp>
#include <cstdarg>
#include <cstdio>

TextTerminal::~TextTerminal() = default;

TextTerminal::TextTerminal()
 : display_(nullptr),
   cursor_{0, 0}
{}

void TextTerminal::init(TextDisplayDevice* display)
{
    assert(display);
    display_ = display;

    for (int i = 0; i < height(); ++i) {
        lines_.push_back(TextLine());
    }
}

void TextTerminal::putchar(char ch)
{
    if (ch == 0) {
        // do nothing
    } else if (ch == '\n') {
        move_cursor_for_newline();
    } else if (ch == '\b') {
        backspace();
    } else {
        put_normal_character(ch);
    }
    set_display_cursor_position();
}

void TextTerminal::backspace()
{
    advance_cursor_backward();
    lines_.at(cursor_.y).remove(cursor_.x);
    redraw_current_line();
}

void TextTerminal::put_normal_character(char ch)
{
    if (cursor_.y >= height()) {
        cursor_.y = height()-1;
        scroll_one_line();
    }
    insert_char(ch);
    advance_cursor_forward();
}

void TextTerminal::scroll_one_line()
{
    lines_.pop_front();
    lines_.push_back(TextLine());
    for (int i = 0; i < height(); ++i) {
        redraw_line(i);
    }
}

void TextTerminal::insert_char(char ch)
{
    lines_.at(cursor_.y).insert(cursor_.x, ch);
    redraw_current_line();
}

void TextTerminal::redraw_current_line()
{
    redraw_line(cursor_.y);
}

void TextTerminal::redraw_line(int y)
{
    assert(y >= 0);
    lines_.at(y).draw(*display_, y);
}

void TextTerminal::draw_char(const Point2 &pos, char ch)
{
    display_->draw_char(pos, display_->make_char(ch));
}

void TextTerminal::move_cursor_for_newline()
{
    cursor_.x = 0;
    cursor_.y++;
}

void TextTerminal::set_display_cursor_position()
{
    display_->set_cursor_position(cursor_);
}

void TextTerminal::advance_cursor_forward()
{
    cursor_.x++;
    if (cursor_.x >= width()) {
        move_cursor_for_newline();
    }
    set_display_cursor_position();
}

void TextTerminal::advance_cursor_backward()
{
    if (cursor_.x > 0) {
        cursor_.x--;
    }
}

int TextTerminal::width()
{
    return display_->dimensions().width;
}

int TextTerminal::height()
{
    return display_->dimensions().height;
}

void TextTerminal::puts(const char* s)
{
    if (s) while (*s) {
        putchar(*s++);
    }
}

int TextTerminal::printf(const char* fmt, ...)
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

void TextTerminal::move_cursor_left()
{
    advance_cursor_backward();
    set_display_cursor_position();
}

void TextTerminal::move_cursor_right()
{
    cursor_.x++;
    const int line_width = get_line_width();
    if (cursor_.x > line_width) {
        cursor_.x = line_width;
    }
    set_display_cursor_position();
}

void TextTerminal::move_cursor_to_end()
{
    cursor_.x = get_line_width();
    set_display_cursor_position();
}

int TextTerminal::get_line_width()
{
    return lines_.at(cursor_.y).size();
}
