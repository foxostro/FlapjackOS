#pragma once

#include <common/text_display_device.hpp>

class dummy_text_display_device : public text_display_device {
    vgachar_t _buffer[CONSOLE_WIDTH*CONSOLE_HEIGHT];
    point2_t _cursor;

public:
    dummy_text_display_device()
    {
        clear();
    }

    void clear() override
    {
        memset(_buffer, 0, sizeof(_buffer));
    }

    void draw_char(point2_t pos, vgachar_t ch) override
    {
        if (pos.x >= 0 && pos.y >= 0 && pos.y < CONSOLE_HEIGHT && pos.x < CONSOLE_WIDTH) {
            size_t i = pos.x + CONSOLE_WIDTH * pos.y;
            assert(i < CONSOLE_WIDTH * CONSOLE_HEIGHT);
            _buffer[i] = ch;
        }
    }

    vgachar_t get_char(point2_t pos) const override
    {
        size_t i = pos.x + CONSOLE_WIDTH * pos.y;
        assert(i < CONSOLE_WIDTH * CONSOLE_HEIGHT);
        return _buffer[i];
    }

    vgachar_t make_char(char ch) const override
    {
        vgachar_t r;
        r.blink = 0;
        r.fg = WHITE;
        r.bg = BLACK;
        r.ch = ch;
        return r;
    }

    void set_cursor_position(point2_t pos) override
    {
        _cursor = pos;
    }

    point2_t get_cursor_position() const override
    {
        return _cursor;
    }

    size_t get_tab_width() const override
    {
        return TAB_WIDTH;
    }

    size2_t dimensions() const override
    {
        return {CONSOLE_WIDTH, CONSOLE_HEIGHT};
    }

    std::string get_line(size_t row)
    {
        assert(row < CONSOLE_HEIGHT);
        std::string str;
        point2_t pos{0, (int)row};
        for (pos.x = 0; pos.x < CONSOLE_WIDTH; ++pos.x) {
            str += get_char(pos).ch;
        }
        return str;
    }
};
