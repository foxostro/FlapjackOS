#include <common/text_terminal.hpp>
#include <common/text_display_device.hpp>
#include <common/text_line.hpp>
#include <common/minmax.hpp>
#include <cstdarg>
#include <cstdio>
#include <cctype>

text_terminal::~text_terminal() = default;

text_terminal::text_terminal(text_display_device &disp)
 : _display(disp)
{}

void text_terminal::draw() const
{
    vector<text_line> lines = get_lines();

    int row = 0;
    for (int i = MAX(lines.size() - CONSOLE_HEIGHT, 0); i < lines.size(); ++i) {
        lines[i].draw(_display, row++);
    }

    // Clear the remainder of the display.
    auto space = _display.make_char(' ');
    for (; row < CONSOLE_HEIGHT; ++row) {
        for (int col = 0; col < CONSOLE_WIDTH; ++col) {
            _display.draw_char(row, col, space);
        }
    }
}

vector<text_line> text_terminal::get_lines() const
{
    vector<text_line> lines;

    text_buffer::size_type i = 0;

    while (i < _buffer.size()) {
        text_line line(CONSOLE_WIDTH, TAB_WIDTH);

        // Insert into the line until it's full.
        bool done = false;
        while (!done) {
            if (i >= _buffer.size()) {
                // We've reached the end of the buffer and there's nothing left.
                done = true;
            } else {
                const char ch = _buffer[i];
                
                if (ch == '\n') {
                    // Start a new line on '\n'. Consume the character too.
                    ++i;
                    done = true;
                } else if (line.push_back(ch)) {
                    // We were able to fit the character into the line.
                    // Keep going.
                    ++i;
                } else {
                    // We were not able to fit the character into the line.
                    // We'll emit the line we have now and start a new one.
                    done = true;
                }
            }
        }

        // Emit the line.
        lines.push_back(std::move(line));
    }

    return lines;
}

void text_terminal::putchar(char ch)
{
    _buffer.insert(ch);
    draw();
}

void text_terminal::puts(const char *s)
{
    _buffer.insert(s);
    draw();
}

void text_terminal::puts(const vector<char> &s)
{
    _buffer.insert(s);
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
