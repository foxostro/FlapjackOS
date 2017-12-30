#include <common/text_buffer.hpp>
#include <cctype>

text_buffer::cursor_position::cursor_position(const text_buffer &owner)
 : _owner(owner), _position(0)
{}

bool text_buffer::cursor_position::move_forward()
{
    if (_position == 0) {
        return false;
    } else {
        _position--;
        return true;
    }
}

bool text_buffer::cursor_position::move_backward()
{
    if (_position == _owner.size()) {
        return false;
    } else {
        _position++;
        return true;
    }
}

text_buffer::size_type text_buffer::cursor_position::as_index() const
{
    const text_buffer::size_type index = _owner.size() - _position;
    assert(index >= 0 && index <= _owner.size());
    return index;
}

void text_buffer::cursor_position::set(size_type index)
{
    assert(index >= 0 && index <= _owner.size());
    _position = _owner.size() - index;
}

text_buffer::~text_buffer() = default;

text_buffer::text_buffer() : _cursor(*this) {}

text_buffer::cursor_position& text_buffer::get_cursor()
{
    return _cursor;
}

void text_buffer::insert(char c)
{
    if (c == '\b') {
        if (_cursor.as_index() > 0) {
            _data.remove(_cursor.as_index() - 1);
            _cursor.move_backward();
        }
    } else if (isprint(c)) {
        _data.insert(_cursor.as_index(), c);
        _cursor.move_forward();
    }
}

void text_buffer::insert(const value_type *string)
{
    if (string) {
        while(*string) {
            insert(*string++);
        }
    }
}

void text_buffer::insert(const vector<value_type> &string)
{
    for (vector<value_type>::size_type i = 0, n = string.size(); i < n; ++i) {
        insert(string[i]);
    }
}

text_buffer::size_type text_buffer::size() const
{
    return _data.size();
}

text_buffer::value_type& text_buffer::operator[](size_type i)
{
    return _data[i];
}

text_buffer::value_type text_buffer::operator[](size_type i) const
{
    return _data[i];
}

void text_buffer::discard(const cursor_position &pos)
{
    for (size_type i = 0, n = pos.as_index(); i < n; ++i) {
        _data.pop_front();
    }
}
