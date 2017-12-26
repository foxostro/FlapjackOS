#include <common/text_terminal.hpp>
#include <cstdarg>
#include <cstdio>
#include <cctype>

text_terminal::~text_terminal() = default;

text_terminal::text_terminal(text_display_device &disp)
 : display(disp), cursor_row(0), cursor_col(0)
{}

void text_terminal::move_back_one(int &row, int &col)
{
    if (col == 0 && row == 0) {
        return;
    }

    if (col == 0) {
        col = CONSOLE_WIDTH-1;

        if (row > 0) {
            row--;
        }
    } else {
        col--;
    }
}

void text_terminal::backspace(int &row, int &col)
{
    move_back_one(row, col);
    display.draw_char(row, col, display.make_char(' '));
}

void text_terminal::newline(int &row, int &col)
{
    if (row == (int)CONSOLE_HEIGHT-1) {
        for (int r = 1; r <= (int)CONSOLE_HEIGHT; r++) {
            for (int c = 0; c < (int)CONSOLE_WIDTH; c++) {
                vgachar_t ch = display.get_char(r, c);
                display.draw_char(r - 1, c, ch);
            }
        }
    } else {
        row++;
    }

    col = 0;

    for (int c = 0; c < (int)CONSOLE_WIDTH; c++) {
        display.draw_char(row, c, display.make_char(' '));
    }
}

void text_terminal::enter_character(int &row, int &col, char ch)
{
    if (isprint(ch)) {
        display.draw_char(row, col, display.make_char(ch));
        col++;
        if (col == (int)CONSOLE_WIDTH) {
            newline(row, col);
        }
    }
}

void text_terminal::putchar(char ch)
{
    int row = get_cursor_row();
    int col = get_cursor_col();

    switch (ch) {
        case '\b':
            backspace(row, col);
            break;

        case '\n':
            newline(row, col);
            break;

        case '\t':
            // TODO: better handling of tab character
            enter_character(row, col, ' ');
            break;

        default:
            enter_character(row, col, ch);
            break;
    };

    set_cursor_position(row, col);
}

void text_terminal::puts(const char *s)
{
    while (*s) {
        putchar(*s++);
    }
}

void text_terminal::puts(vector<char> &str)
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

void text_terminal::set_cursor_position(int row, int col)
{
    assert(row >= 0);
    assert(row < (int)CONSOLE_HEIGHT);
    assert(col >= 0);
    assert(col < (int)CONSOLE_WIDTH);
    cursor_row = row;
    cursor_col = col;
    display.set_cursor_position(cursor_row, cursor_col);
}

int text_terminal::get_cursor_row() const
{
    return cursor_row;
}

int text_terminal::get_cursor_col() const
{
    return cursor_col;
}

void text_terminal::move_cursor_left()
{
    move_back_one(cursor_row, cursor_col);
    set_cursor_position(cursor_row, cursor_col);
}

void text_terminal::move_cursor_right()
{
    if (cursor_col >= (int)CONSOLE_WIDTH || cursor_row >= (int)CONSOLE_HEIGHT) {
        return;
    }

    cursor_col++;
    if (cursor_col == (int)CONSOLE_WIDTH) {
        if (cursor_row < (int)CONSOLE_HEIGHT-1) {
            cursor_row++;
        }

        cursor_col = 0;
    }

    set_cursor_position(cursor_row, cursor_col);
}
