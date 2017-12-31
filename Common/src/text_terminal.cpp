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
   _logical_cursor_row(0)
   // ,_logical_cursor_col(0)
{}

void text_terminal::draw() const
{
    // TODO: draw only what has changed.
    // TODO: scroll the display if we can't fit everything.

    int row = 0;
    for (int i = 0, n = _logical_lines.size(); i < n; ++i) {
        const auto& line = _logical_lines[i];
        row = line.draw(_display, row);
    }

    // Clear the remainder of the display.
    auto space = _display.make_char(' ');
    for (; row < CONSOLE_HEIGHT; ++row) {
        for (int col = 0; col < CONSOLE_WIDTH; ++col) {
            _display.draw_char(row, col, space);
        }
    }
}

void text_terminal::_putchar(char ch)
{
    if (_logical_lines.empty()) {
        text_line line(CONSOLE_WIDTH, TAB_WIDTH);
        _logical_lines.push_back(line);
        _logical_cursor_row = 0;
    }

    if (ch == '\n') {
        text_line line(CONSOLE_WIDTH, TAB_WIDTH);

        if (_logical_lines.full()) {
            _logical_lines.pop_front();
        }

        _logical_lines.push_back(line);
        _logical_cursor_row++;
    } else {
        // TODO: insert at the logical-cursor-column position
        // TODO: handle backspace too
        auto &line = _logical_lines[_logical_cursor_row];
        line.push_back(ch);
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
