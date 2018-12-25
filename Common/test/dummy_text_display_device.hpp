#ifndef FLAPJACKOS_COMMON_TEST_DUMMY_TEXT_DISPLAY_DEVICE_HPP
#define FLAPJACKOS_COMMON_TEST_DUMMY_TEXT_DISPLAY_DEVICE_HPP

#include <common/text_display_device.hpp>

class DummyTextDisplayDevice : public TextDisplayDevice {
    VGAChar _buffer[CONSOLE_WIDTH*CONSOLE_HEIGHT];
    Point2 _cursor;

public:
    DummyTextDisplayDevice()
    {
        clear();
    }

    void clear() override
    {
        memset(_buffer, 0, sizeof(_buffer));
    }

    void draw_char(Point2 pos, VGAChar ch) override
    {
        if (pos.x >= 0 && pos.y >= 0 && pos.y < CONSOLE_HEIGHT && pos.x < CONSOLE_WIDTH) {
            size_t i = pos.x + CONSOLE_WIDTH * pos.y;
            assert(i < CONSOLE_WIDTH * CONSOLE_HEIGHT);
            _buffer[i] = ch;
        }
    }

    VGAChar get_char(Point2 pos) const override
    {
        size_t i = pos.x + CONSOLE_WIDTH * pos.y;
        assert(i < CONSOLE_WIDTH * CONSOLE_HEIGHT);
        return _buffer[i];
    }

    VGAChar make_char(char ch) const override
    {
        VGAChar r;
        r.attr.blink = 0;
        r.attr.fg = WHITE;
        r.attr.bg = BLACK;
        r.attr.ch = ch;
        return r;
    }

    void set_cursor_position(Point2 pos) override
    {
        _cursor = pos;
    }

    Point2 get_cursor_position() const override
    {
        return _cursor;
    }

    size_t get_tab_width() const override
    {
        return TAB_WIDTH;
    }

    Size2 dimensions() const override
    {
        return {CONSOLE_WIDTH, CONSOLE_HEIGHT};
    }

    std::string get_line(size_t row)
    {
        assert(row < CONSOLE_HEIGHT);
        std::string str;
        Point2 pos{0, (int)row};
        for (pos.x = 0; pos.x < CONSOLE_WIDTH; ++pos.x) {
            char ch = get_char(pos).attr.ch;
            if (ch == 0) {
                break;
            } else {
                str += ch;
            }
        }
        return str;
    }
};

#endif // FLAPJACKOS_COMMON_TEST_DUMMY_TEXT_DISPLAY_DEVICE_HPP
