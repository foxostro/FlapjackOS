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

text_line::text_line(const text_buffer *owner, int tab_width)
 : _owner(owner),
   _tab_width(tab_width),
   begin(owner),
   end(owner)
{
    assert(_owner);
    assert(tab_width >= 0);
}

void text_line::draw(text_display_device &display, int row) const
{
    assert(begin.as_index() <= end.as_index());

    int col = 0;

    for (text_buffer::size_type i = begin.as_index(); i <= end.as_index(); ++i) {
        const char ch = _owner->at(i);
        display.draw_char(row, col, display.make_char(ch));
        col += step_for_char(_tab_width, col, ch);
    }
}

int text_line::columns() const
{
    assert(begin.as_index() <= end.as_index());

    int col = 0;

    for (text_buffer::size_type i = begin.as_index(); i <= end.as_index(); ++i) {
        assert(i >= 0 && i < _owner->size());
        col += step_for_char(_tab_width, col, _owner->at(i));
    }

    return col;
}

vector<char> text_line::get() const
{
    assert(begin.as_index() <= end.as_index());

    vector<char> data;

    for (text_buffer::size_type i = begin.as_index(); i <= end.as_index(); ++i) {
        const char ch = _owner->at(i);
        data.push_back(ch);
    }

    data.push_back('\0');

    return data;
}
