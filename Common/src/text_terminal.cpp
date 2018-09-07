#include <common/text_terminal.hpp>
#include <common/text_display_device.hpp>
#include <common/text_line.hpp>
#include <common/minmax.hpp>
#include <cstdarg>
#include <cstdio>
#include <cctype>

TextTerminal::~TextTerminal() = default;

TextTerminal::TextTerminal(TextDisplayDevice &disp)
 : display_(disp),
   logical_cursor_{0, 0}
{}

void TextTerminal::draw()
{
    int cursor_offset = 0;

    // Count the number of display lines needed to show all logical lines.
    int rows_needed = 0;
    for (int i = 0, n = logical_lines_.size(); i < n; ++i) {
        const auto& line = logical_lines_[i];
        rows_needed += line.get_cached_display_size().height;
    }

    // Start drawing above the top of the display so that the bottom lines
    // appear on screen.
    int row;
    if (rows_needed >= CONSOLE_HEIGHT) {
        row = CONSOLE_HEIGHT - rows_needed;
    } else {
        row = 0;
    }

    // Draw all dirty lines. Skip all non-dirty lines to save time.
    for (int i = 0, n = logical_lines_.size(); i < n; ++i) {
        auto& line = logical_lines_[i];
        if (line.dirty) {
            line.draw(row);
        }
        if (i == logical_cursor_.y) {
            cursor_offset = row;
        }
        row += line.get_cached_display_size().height;
    }

    // Clear the remainder of the display.
    auto space = display_.make_char(' ');
    for (; row < CONSOLE_HEIGHT; ++row) {
        for (int col = 0; col < CONSOLE_WIDTH; ++col) {
            display_.draw_char(Point2{col, row}, space);
        }
    }

    // Set the hardware cursor position using physical display coords.
    Point2 phys_cursor = logical_lines_[logical_cursor_.y].convert(logical_cursor_.x);
    phys_cursor.y += cursor_offset;
    display_.set_cursor_position(phys_cursor);
}

void TextTerminal::putchar_internal(char ch)
{
    bool previous_are_dirty_too = false;
    bool subsequent_are_dirty_too = false;

    if (logical_lines_.empty()) {
        TextLine line(display_);
        logical_lines_.push_back(line);
        logical_cursor_.y = 0;
        logical_cursor_.x = 0;
    }

    if (ch == '\n') {
        TextLine line(display_);

        if (logical_lines_.full()) {
            logical_lines_.pop_front();
            logical_cursor_.y--;

            // Mark all previous lines as dirty because we are scrolling and
            // they will also be affected.
            previous_are_dirty_too = true;
        }

        logical_lines_.push_back(line);
        logical_cursor_.y++;
        logical_cursor_.x = 0;

        // Since we added a line, all subsequent lines will have moved and
        // should be redrawn.
        subsequent_are_dirty_too = true;
    } else {
        auto &line = logical_lines_[logical_cursor_.y];

        Size2 old_phys_size = line.get_cached_display_size();

        if (ch == '\b') {
            if (logical_cursor_.x > 0) {
                line.remove(--logical_cursor_.x);
            }
        } else {
            // If the line is full then no insertion is performed.
            if (line.insert(logical_cursor_.x, ch)) {
                logical_cursor_.x++;
            }
        }

        if (old_phys_size != line.get_cached_display_size()) {
            // Mark all subsequent lines as dirty because we are now overflowing
            // into an additional display line.
            subsequent_are_dirty_too = true;
        }
    }

    if (previous_are_dirty_too) {
        for (int i = 0; i <=logical_cursor_.y; ++i) {
            logical_lines_[i].dirty = true;
        }
    }

    if (subsequent_are_dirty_too) {
        for (int i = logical_cursor_.y; i < logical_lines_.size(); ++i) {
            logical_lines_[i].dirty = true;
        }
    }
}

void TextTerminal::putchar(char ch)
{
    putchar_internal(ch);
    draw();
}

void TextTerminal::puts(const char *s)
{
    while (*s) {
        putchar_internal(*s++);
    }
    draw();
}

int TextTerminal::printf(const char *fmt, ...)
{
    char buffer[128] = {0};

    va_list args;
    va_start(args, fmt);
    int r = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    puts(buffer);

    return r;
}

void TextTerminal::move_cursor_left()
{
    if (logical_cursor_.x > 0) {
        logical_cursor_.x--;
        draw();
    }
}

void TextTerminal::move_cursor_right()
{
    const auto &line = logical_lines_[logical_cursor_.y];
    if (logical_cursor_.x < line.size()) {
        logical_cursor_.x++;
        draw();
    }
}

void TextTerminal::move_cursor_to_end()
{
    const auto &line = logical_lines_[logical_cursor_.y];
    if (logical_cursor_.x != line.size()) {
        logical_cursor_.x = line.size();
        draw();
    }
}
