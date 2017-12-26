#pragma once

#include <common/console_device.hpp>

class vga_console_device : public console_device {
    size_t cursor_row, cursor_col,
           curr_fg, curr_bg;

    inline vgachar_t space_character() const
    {
        return make_char(' ');
    }

    void newline();
    void tab();
    void backspace();

public:
	// Constructor.
    vga_console_device();

    void clear() override;
    void draw_char(size_t row, size_t col, vgachar_t ch) override;
    vgachar_t get_char(size_t row, size_t col) const override;
    vgachar_t make_char(char ch) const override;
    void set_cursor_position(size_t row, size_t col) override;
    size_t get_cursor_row() const override;
    size_t get_cursor_col() const override;
};
