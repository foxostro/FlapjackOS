#include <common/text_line.hpp>
#include <common/terminal_metrics.hpp>
#include <common/text_display_device.hpp>
#include <common/minmax.hpp>
#include <cassert>
#include <cctype>

TextLine::TextLine() = default;
TextLine::TextLine(const TextLine& line) = default;
TextLine::TextLine(TextLine&& line) : data_(std::move(line.data_)) {}

TextLine& TextLine::operator=(const TextLine& other)
{
    if (this != &other) {
        data_ = other.data_;
    }
    return *this;
}

TextLine& TextLine::operator=(TextLine&& other)
{
    if (this != &other) {
        data_ = std::move(other.data_);
    }
    return *this;
}

int TextLine::draw(TextDisplayDevice& display, int row)
{
    int col = 0;
    const int tab_width = display.get_tab_width();
    const int max_columns = display.dimensions().width;

    for (int i = 0; i < data_.size(); ++i) {
        char ch = data_[i];
        int step = step_for_char(tab_width, col, ch);
        VGAChar vgachar = display.make_char((ch=='\t') ? ' ' : ch);

        for (int j = col; j < MIN(col+step, max_columns); ++j) {
            display.draw_char(Point2{j, row}, vgachar);
        }

        col += step;
        if (col >= max_columns) {
            col = 0;
            row++;
        }
    }

    const auto space = display.make_char(0);
    for (; col < max_columns; ++col) {
        display.draw_char(Point2{col, row}, space);
    }

    return row + 1;
}

int TextLine::get_physical_column_for_index(TextDisplayDevice& display, size_type index)
{
    int col = 0;
    for (size_type i = 0; i < index; ++i) {
        col += step_for_char(display.get_tab_width(), col, data_[i]);
    }
    return col;
}

Vector<char> TextLine::str() const
{
    Vector<char> chars;
    for (int i = 0; i < data_.size(); ++i) {
        chars.push_back(data_[i]);
    }
    chars.push_back(0);
    return chars;
}

bool TextLine::push_back(char ch)
{
    assert(ch != '\n');
    assert(ch != '\b');
    return data_.push_back(ch);
}

void TextLine::pop_back()
{
    if (!data_.empty()) {
        data_.pop_back();
    }
}

bool TextLine::insert(size_type index, char ch)
{
    assert(ch != '\n');
    assert(ch != '\b');
    return data_.insert(index, ch);
}

void TextLine::remove(size_type index)
{
    if (!data_.empty()) {
        data_.remove(index);
    }
}

TextLine::size_type TextLine::size() const
{
    return data_.size();
}

int TextLine::step_for_char(int tab_width, int col, char ch)
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
