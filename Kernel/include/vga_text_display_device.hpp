#ifndef FLAPJACKOS_KERNEL_INCLUDE_VGA_TEXT_DISPLAY_DEVICE_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_VGA_TEXT_DISPLAY_DEVICE_HPP

#include <common/text_display_device.hpp>

// VGA text display output driver.
class VGATextDisplayDevice : public TextDisplayDevice {
public:
	// Constructor.
    VGATextDisplayDevice();

    void clear() override;
    void draw_char(Point2 pos, VGAChar ch) override;
    VGAChar get_char(Point2 pos) const override;
    VGAChar make_char(char ch) const override;
    void set_cursor_position(Point2 pos) override;
    Point2 get_cursor_position() const override;
    size_t get_tab_width() const override;
    Size2 dimensions() const override;

private:
    Point2 cursor_pos_;
    size_t current_foreground_color_, current_background_color_;
    
    inline VGAChar space_character() const
    {
        return make_char(' ');
    }
    
    void newline();
    void tab();
    void backspace();
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_VGA_TEXT_DISPLAY_DEVICE_HPP
