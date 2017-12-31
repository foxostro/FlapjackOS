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
   _cached_display_size{0, 0},
   dirty(true)
{
    assert(max_columns > 0);
    assert(tab_width > 0);
}

text_line::text_line(const text_line &line)
 : _data(line._data),
   _max_columns(line._max_columns),
   _tab_width(line._tab_width),
   _cached_display_size(line._cached_display_size),
   dirty(true)
{}

text_line::text_line(text_line &&line)
 : _data(std::move(line._data)),
   _max_columns(line._max_columns),
   _tab_width(line._tab_width),
   _cached_display_size(line._cached_display_size),
   dirty(true)
{}

text_line& text_line::operator=(const text_line &other)
{
    if (this != &other) {
        _data = other._data;
        _max_columns = other._max_columns;
        _tab_width = other._tab_width;
        _cached_display_size = other._cached_display_size;
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
        _cached_display_size = other._cached_display_size;
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
            display.draw_char(point2_t{j, row}, vgachar);
        }

        col += step;
        if (col >= _max_columns) {
            col = 0;
            row++;
        }
    }

    const auto space = display.make_char(' ');
    for (; col < _max_columns; ++col) {
        display.draw_char(point2_t{col, row}, space);
    }

    return row + 1;
}

size2_t text_line::measure() const
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

    return size2_t{max_col, row + 1};
}

point2_t text_line::convert(int logi_col) const
{
    int col = 0, row = 0;

    for (int i = 0; i < _data.size(); ++i) {
        char ch = _data[i];
        int step = step_for_char(_tab_width, col, ch);

        if (i == logi_col) {
            goto done;
        }

        col += step;
        if (col >= _max_columns) {
            col = 0;
            row++;
        }
    }

done:
    return point2_t{col, row};
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
    _cached_display_size = measure();
    return r;
}

text_line::size_type text_line::size() const
{
    return _data.size();
}

size2_t text_line::get_cached_display_size() const
{
    return _cached_display_size;
}
