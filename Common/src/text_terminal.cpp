#include <common/text_terminal.hpp>
#include <common/text_display_device.hpp>
#include <common/text_line.hpp>
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

    _display.clear();

    for (int row = 0; row < CONSOLE_HEIGHT; ++row) {
        int i = (lines.size() - CONSOLE_HEIGHT) + row;
        lines[i].draw(_display, row);
    }
}

vector<text_line> text_terminal::get_lines() const
{
    ::printf("text_terminal::get_lines()\n");

    vector<text_line> lines;

    text_buffer::size_type i = 0;

    while (i < _buffer.size()-1) {
        ::printf("getting a new line... i=%d\n", i);
        text_line line(&_buffer, TAB_WIDTH);
        line.begin.set(i);
        
        // Move the end of the line until we reach the edge of the display, or
        // the end of the buffer.
        while (i < _buffer.size()-1) {
            line.end.set(++i);

            if (line.columns() > CONSOLE_WIDTH) {
                // Too big. Undo the increment and emit the line.
                line.end.set(--i);
                ::printf("found line limits; i=%d\n", i);
                break;
            }
        }

        // Emit the line.
        ::printf("emit line: \"%s\"\n", line.get().data());
        lines.push_back(line);
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
