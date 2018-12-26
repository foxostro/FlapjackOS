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
        lines_.push_back(TextLine(*display));
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
}

void TextTerminal::backspace()
{
    advance_cursor_backward();
    lines_.at(cursor_.y).remove(cursor_.x);
    redraw_current_line();
}

void TextTerminal::put_normal_character(char ch)
{
    insert_char(ch);
    advance_cursor_forward();
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
    Point2 pos{0, y};
    auto& line = lines_.at(pos.y);
    for (pos.x = 0; pos.x < width(); ++pos.x) {
        char ch = get_line_character(line, pos.x);
        draw_char(pos, ch);
    }
}

char TextTerminal::get_line_character(TextLine& line, int column)
{
    char ch;
    if (column < line.size()) {
        ch = line.at(column);
    } else {
        ch = 0;
    }
    return ch;
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
    return cursor_.x >= width();
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
}

void TextTerminal::move_cursor_right()
{
    cursor_.x++;
    if (cursor_.x > width()) {
        cursor_.x = width();
    }
}

void TextTerminal::move_cursor_to_end() {}
