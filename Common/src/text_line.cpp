#include <common/text_line.hpp>
#include <common/terminal_metrics.hpp>
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

text_line::text_line()
 : text_line(CONSOLE_WIDTH, TAB_WIDTH)
{}

text_line::text_line(int max_columns, int tab_width)
 : _max_columns(max_columns), _tab_width(tab_width)
{
    assert(max_columns > 0);
    assert(tab_width > 0);
}

const vector<char>& text_line::data() const
{
    return _data;
}

int text_line::columns() const
{
    return for_each_col([](int,int,char){});
}

vector<char> text_line::insert(int position, char ch)
{
    vector<char> chars(1);
    chars[0] = ch;
    return insert(position, chars);
}

vector<char> text_line::insert(int position, vector<char> chars)
{
    assert(position >= 0 && position <= _data.size());

    for (int i = 0, n = chars.size(); i < n; ++i) {
        _data.insert(position, chars.pop_back());
    }

    // Find the position of the last character after the maximum column.
    int char_pos = 0;
    for_each_col([this, &char_pos](int col, int, char){
        if (col < _max_columns) {
            char_pos++;
        }
    });

    // Take the characters after the maximum column and move them to `overflow'.
    vector<char> overflow;
    for (int i = char_pos; i < _data.size(); ++i) {
        overflow.push_back(_data[i]);
    }
    while (_data.size() > char_pos) {
        _data.pop_back();
    }

    return overflow;
}

void text_line::remove(int position)
{
    assert(position >= 0 && position < _data.size());
    _data.remove(position);
}

int text_line::pos_for_col(int col) const
{
    int pos = 0;
    for_each_col([&pos, col](int column, int, char){
        if (column <= col) {
            ++pos;
        }
    });
    return pos;
}
