#include <common/text_terminal.hpp>
#include <common/text_display_device.hpp>
#include <common/text_line.hpp>
#include <common/minmax.hpp>
#include <cstdarg>
#include <cstdio>
#include <cctype>

text_terminal::~text_terminal() = default;

text_terminal::text_terminal(text_display_device &disp)
 : _display(disp),
   _logical_cursor{0, 0}
{}

void text_terminal::draw()
{
    int cursor_offset = 0;

    // Count the number of display lines needed to show all logical lines.
    int rows_needed = 0;
    for (int i = 0, n = _logical_lines.size(); i < n; ++i) {
        const auto& line = _logical_lines[i];
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
    for (int i = 0, n = _logical_lines.size(); i < n; ++i) {
        auto& line = _logical_lines[i];
        if (line.dirty) {
            line.draw(_display, row);
        }
        if (i == _logical_cursor.y) {
            cursor_offset = row;
        }
        row += line.get_cached_display_size().height;
    }

    // Clear the remainder of the display.
    auto space = _display.make_char(' ');
    for (; row < CONSOLE_HEIGHT; ++row) {
        for (int col = 0; col < CONSOLE_WIDTH; ++col) {
            _display.draw_char(point2_t{col, row}, space);
        }
    }

    // Set the hardware cursor position using physical display coords.
    point2_t phys_cursor = _logical_lines[_logical_cursor.y].convert(_logical_cursor.x);
    phys_cursor.y += cursor_offset;
    _display.set_cursor_position(phys_cursor);
}

void text_terminal::_putchar(char ch)
{
    bool previous_are_dirty_too = false;
    bool subsequent_are_dirty_too = false;

    if (_logical_lines.empty()) {
        text_line line(CONSOLE_WIDTH, TAB_WIDTH);
        _logical_lines.push_back(line);
        _logical_cursor.y = 0;
        _logical_cursor.x = 0;
    }

    if (ch == '\n') {
        text_line line(CONSOLE_WIDTH, TAB_WIDTH);

        if (_logical_lines.full()) {
            _logical_lines.pop_front();
            _logical_cursor.y--;

            // Mark all previous lines as dirty because we are scrolling and
            // they will also be affected.
            previous_are_dirty_too = true;
        }

        _logical_lines.push_back(line);
        _logical_cursor.y++;
        _logical_cursor.x = 0;

        // Since we added a line, all subsequent lines will have moved and
        // should be redrawn.
        subsequent_are_dirty_too = true;
    } else {
        // TODO: insert at the logical-cursor-column position
        // TODO: handle backspace too
        auto &line = _logical_lines[_logical_cursor.y];

        size2_t old_phys_size = line.get_cached_display_size();

        line.push_back(ch);
        _logical_cursor.x = line.size();

        if (old_phys_size != line.get_cached_display_size()) {
            // Mark all subsequent lines as dirty because we are now overflowing
            // into an additional display line.
            subsequent_are_dirty_too = true;
        }
    }

    if (previous_are_dirty_too) {
        for (int i = 0; i <=_logical_cursor.y; ++i) {
            _logical_lines[i].dirty = true;
        }
    }

    if (subsequent_are_dirty_too) {
        for (int i = _logical_cursor.y; i < _logical_lines.size(); ++i) {
            _logical_lines[i].dirty = true;
        }
    }
}

void text_terminal::putchar(char ch)
{
    _putchar(ch);
    draw();
}

void text_terminal::puts(const char *s)
{
    while (*s) {
        _putchar(*s++);
    }
    draw();
}

void text_terminal::puts(const vector<char> &s)
{
    for (vector<char>::size_type i = 0; i < s.size(); ++i) {
        _putchar(s[i]);
    }
    draw();
}

int text_terminal::printf(const char *fmt, ...)
{
    char buffer[128] = {0};

    va_list args;
    va_start(args, fmt);
    int r = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    puts(buffer);

    return r;
}
