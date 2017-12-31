#include <common/text_line.hpp>
#include <common/terminal_metrics.hpp>
#include <common/text_display_device.hpp>
#include <common/minmax.hpp>
#include <cassert>
#include <cctype>

int text_line::step_for_char(int tab_width, int col, char ch)
{
    int delta;
    if (ch == '\t') {
        delta = tab_width - (col % tab_width);
    } else if (isprint(ch)) {
        delta = 1;
    } else {
        delta = 0;
    }
    return delta;
}

text_line::~text_line() = default;

text_line::text_line()
 : text_line(CONSOLE_WIDTH, TAB_WIDTH)
{}

text_line::text_line(int max_columns, int tab_width)
 : _max_columns(max_columns),
   _tab_width(tab_width),
   dirty(true)
{
    assert(max_columns > 0);
    assert(tab_width > 0);
}

text_line::text_line(const text_line &line)
 : _data(line._data),
   _max_columns(line._max_columns),
   _tab_width(line._tab_width),
   dirty(true)
{}

text_line::text_line(text_line &&line)
 : _data(std::move(line._data)),
   _max_columns(line._max_columns),
   _tab_width(line._tab_width),
   dirty(true)
{}

text_line& text_line::operator=(const text_line &other)
{
    if (this != &other) {
        _data = other._data;
        _max_columns = other._max_columns;
        _tab_width = other._tab_width;
        dirty = other.dirty;
    }

    return *this;
}

text_line& text_line::operator=(text_line &&other)
{
    if (this != &other) {
        _data = std::move(other._data);
        _max_columns = other._max_columns;
        _tab_width = other._tab_width;
        dirty = other.dirty;
    }

    return *this;
}

int text_line::draw(text_display_device &display, int row)
{
    dirty = false;

    int col = 0;

    for (int i = 0; i < _data.size(); ++i) {
        char ch = _data[i];
        int step = step_for_char(_tab_width, col, ch);
        vgachar_t vgachar = display.make_char((ch=='\t') ? ' ' : ch);

        for (int j = col; j < MIN(col+step, _max_columns); ++j) {
            display.draw_char(row, j, vgachar);
        }

        col += step;
        if (col >= _max_columns) {
            col = 0;
            row++;
        }
    }

    const auto space = display.make_char(' ');
    for (; col < _max_columns; ++col) {
        display.draw_char(row, col, space);
    }

    return row + 1;
}

void text_line::measure(int &out_rows, int &out_columns) const
{
    int col = 0, max_col = 0, row = 0;

    for (int i = 0; i < _data.size(); ++i) {
        const char ch = _data[i];
        col += step_for_char(_tab_width, col, ch);
        if (col >= _max_columns) {
            max_col = MAX(max_col, MIN(col, _max_columns));
            col = 0;
            row++;
        }
    }

    out_columns = max_col;
    out_rows = row;
}

vector<char> text_line::str() const
{
    vector<char> chars;
    for (int i = 0; i < _data.size(); ++i) {
        chars.push_back(_data[i]);
    }
    chars.push_back(0);
    return chars;
}

bool text_line::push_back(char ch)
{
    assert(ch != '\n');
    bool r = _data.push_back(ch);
    dirty = true;
    return r;
}
