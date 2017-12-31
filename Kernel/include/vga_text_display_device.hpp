#pragma once

#include <common/text_display_device.hpp>

// VGA text display output driver.
class vga_text_display_device : public text_display_device {
    point2_t cursor_pos;
    size_t curr_fg, curr_bg;

    inline vgachar_t space_character() const
    {
        return make_char(' ');
    }

    void newline();
    void tab();
    void backspace();

public:
	// Constructor.
    vga_text_display_device();

    void clear() override;
    void draw_char(point2_t pos, vgachar_t ch) override;
    vgachar_t get_char(point2_t pos) const override;
    vgachar_t make_char(char ch) const override;
    void set_cursor_position(point2_t pos) override;
    point2_t get_cursor_position() const override;
    size_t get_tab_width() const override;
    size2_t dimensions() const override;
};
