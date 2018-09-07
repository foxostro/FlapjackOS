#ifndef FLAPJACKOS_KERNEL_INCLUDE_PS2_KEYBOARD_DEVICE_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PS2_KEYBOARD_DEVICE_HPP

#include <common/keyboard_device.hpp>
#include <common/interrupt_handler.hpp>
#include <common/ring_buffer.hpp>

class PS2KeyboardDevice : public KeyboardDevice, public InterruptHandler {
public:
    virtual ~PS2KeyboardDevice() noexcept;
    PS2KeyboardDevice();

    // Keyboard interrupt handler.
    // Implements the lower half of PS/2 keyboard driver. To be called when the
    // keyboard interrupt fires.
    void int_handler() override;

    const char* keycode_name(keycode_t key) override;
    KeyboardEvent get_event() override;
    
private:
    enum KeyboardDriverState {
        IDLE,
        PROCESSING_ESCAPE_CODE
    };
    
    keycode_key_state key_state_[KEYCODE_MAX];
    RingBuffer<KeyboardEvent, 32> events_;
    
    void clear_input();
    bool step_state_machine(KeyboardDriverState &state, KeyboardEvent &output); // TODO: Extract this state machine to its own class.
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_PS2_KEYBOARD_DEVICE_HPP
