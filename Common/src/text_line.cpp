#include <common/text_line.hpp>
#include <common/terminal_metrics.hpp>
#include <common/text_display_device.hpp>
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
 : _max_columns(max_columns), _tab_width(tab_width)
{
    assert(max_columns > 0);
    assert(tab_width > 0);
}

text_line::text_line(const text_line &line)
 : _data(line._data),
   _max_columns(line._max_columns),
   _tab_width(line._tab_width)
{}

text_line::text_line(text_line &&line)
 : _data(std::move(line._data)),
   _max_columns(line._max_columns),
   _tab_width(line._tab_width)
{}

text_line& text_line::operator=(const text_line &other)
{
    if (this != &other) {
        _data = other._data;
        _max_columns = other._max_columns;
        _tab_width = other._tab_width;
    }

    return *this;
}

text_line& text_line::operator=(text_line &&other)
{
    if (this != &other) {
        _data = std::move(other._data);
        _max_columns = other._max_columns;
        _tab_width = other._tab_width;
    }

    return *this;
}

void text_line::draw(text_display_device &display, int row) const
{
    int col = 0;

    for (int i = 0; i < _data.size(); ++i) {
        const char ch = _data[i];
        const auto vgachar = display.make_char(ch);
        int step = step_for_char(_tab_width, col, ch);
        for (int j = 0; j < step; ++j) {
            display.draw_char(row, col + j, vgachar);
        }
        col += step;
    }

    const auto space = display.make_char(' ');
    for (; col < _max_columns; ++col) {
        display.draw_char(row, col, space);
    }
}

int text_line::columns() const
{
    int col = 0;

    for (int i = 0; i < _data.size(); ++i) {
        const char ch = _data[i];
        col += step_for_char(_tab_width, col, ch);
    }

    return col;
}

const vector<char>& text_line::get() const
{
    return _data;
}

vector<char> text_line::str() const
{
    vector<char> s(_data);
    s.push_back(0);
    return s;
}

bool text_line::push_back(char ch)
{
    int col = 0;

    for (int i = 0; i < _data.size(); ++i) {
        col += step_for_char(_tab_width, col, _data[i]);
    }

    col += step_for_char(_tab_width, col, ch);

    if (col <= _max_columns) {
        _data.push_back(ch);
        return true;
    } else {
        return false;
    }
}
