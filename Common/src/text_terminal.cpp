#include <common/text_terminal.hpp>
#include <cstdarg>
#include <cstdio>

UnlockedTextTerminal::~UnlockedTextTerminal() = default;

UnlockedTextTerminal::UnlockedTextTerminal()
 : display_(nullptr),
   cursor_{0, 0}
{}

void UnlockedTextTerminal::init(TextDisplayDevice* display)
{
    assert(display);
    display_ = display;

    for (int i = 0; i < height(); ++i) {
        lines_.push_back(TextLine());
    }
}

void UnlockedTextTerminal::putchar(char ch)
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

void UnlockedTextTerminal::backspace()
{
    advance_cursor_backward();
    lines_.at(cursor_.y).remove(cursor_.x);
    redraw_current_line();
}

void UnlockedTextTerminal::put_normal_character(char ch)
{
    if (cursor_.y >= height()) {
        cursor_.y = height()-1;
        scroll_one_line();
    }
    insert_char(ch);
    advance_cursor_forward();
}

void UnlockedTextTerminal::scroll_one_line()
{
    lines_.pop_front();
    lines_.push_back(TextLine());
    for (int i = 0; i < height(); ++i) {
        redraw_line(i);
    }
}

void UnlockedTextTerminal::insert_char(char ch)
{
    lines_.at(cursor_.y).insert(cursor_.x, ch);
    redraw_current_line();
}

void UnlockedTextTerminal::redraw_current_line()
{
    redraw_line(cursor_.y);
}

void UnlockedTextTerminal::redraw_line(int y)
{
    assert(y >= 0);
    lines_.at(y).draw(*display_, y);
}

void UnlockedTextTerminal::draw_char(const Point2 &pos, char ch)
{
    display_->draw_char(pos, display_->make_char(ch));
}

void UnlockedTextTerminal::move_cursor_for_newline()
{
    cursor_.x = 0;
    cursor_.y++;
}

void UnlockedTextTerminal::set_display_cursor_position()
{
    Point2 physical_cursor;
    physical_cursor.y = cursor_.y;
    physical_cursor.x = lines_.at(cursor_.y).get_physical_column_for_index(*display_, cursor_.x);
    display_->set_cursor_position(physical_cursor);
}

void UnlockedTextTerminal::advance_cursor_forward()
{
    cursor_.x++;
    if (cursor_.x >= width()) {
        move_cursor_for_newline();
    }
    set_display_cursor_position();
}

void UnlockedTextTerminal::advance_cursor_backward()
{
    if (cursor_.x > 0) {
        cursor_.x--;
    }
}

int UnlockedTextTerminal::width()
{
    return display_->dimensions().width;
}

int UnlockedTextTerminal::height()
{
    return display_->dimensions().height;
}

void UnlockedTextTerminal::puts(const char* s)
{
    if (s) while (*s) {
        putchar(*s++);
    }
}

int UnlockedTextTerminal::printf(const char* fmt, ...)
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

void UnlockedTextTerminal::move_cursor_left()
{
    advance_cursor_backward();
    set_display_cursor_position();
}

void UnlockedTextTerminal::move_cursor_right()
{
    cursor_.x++;
    const int line_width = get_line_width();
    if (cursor_.x > line_width) {
        cursor_.x = line_width;
    }
    set_display_cursor_position();
}

void UnlockedTextTerminal::move_cursor_to_end()
{
    cursor_.x = get_line_width();
    set_display_cursor_position();
}

int UnlockedTextTerminal::get_line_width()
{
    return lines_.at(cursor_.y).size();
}
