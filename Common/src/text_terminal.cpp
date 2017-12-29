#include <common/text_terminal.hpp>
#include <cstdarg>
#include <cstdio>
#include <cctype>

text_terminal::~text_terminal() = default;

text_terminal::text_terminal(text_display_device &disp)
 : display(disp),
   lines(CONSOLE_HEIGHT),
   cursor_row(0),
   cursor_col(0)
{}

void text_terminal::insert_char(int row, int col, char ch)
{
    assert(row >= 0);
    assert(row < CONSOLE_HEIGHT);
    assert(col >= 0);
    assert(col < CONSOLE_WIDTH);

    auto &line = lines[row];
    line.insert(line.pos_for_col(col), ch);
    repaint_line(row, line);
}

void text_terminal::putchar(char ch)
{
    insert_char(cursor_row, cursor_col, ch);
    increment_cursor(cursor_row, cursor_col, ch);
    display.set_cursor_position(cursor_row, cursor_col);
}

void text_terminal::puts(const char *s)
{
    while (*s) {
        putchar(*s++);
    }
}

void text_terminal::puts(const vector<char> &str)
{
    for (int i = 0; i < str.size(); ++i) {
        const char ch = str[i];
        if (ch == 0) {
            break;
        }
        putchar(ch);
    }
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

void text_terminal::increment_cursor(int &row, int &col, char ch)
{
    col += text_line::step_for_char(TAB_WIDTH, row, ch);
    if (col >= CONSOLE_WIDTH) {
        col = 0;
        row++;
    }
}

void text_terminal::repaint_line(int row, text_line &line)
{
    for (int i = 0; i < CONSOLE_WIDTH; ++i) {
        display.draw_char(row, i, display.make_char(' '));
    }
    line.for_each_col([this, row](int col, int width, char ch){
        for (int i = 0; i < width; ++i) {
            display.draw_char(row, col+i, display.make_char(ch));
        }
    });
}