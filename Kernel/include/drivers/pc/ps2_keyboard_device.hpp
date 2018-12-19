#ifndef FLAPJACKOS_KERNEL_INCLUDE_DRIVERS_PC_PS2_KEYBOARD_DEVICE_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_DRIVERS_PC_PS2_KEYBOARD_DEVICE_HPP

#include <common/keyboard_device.hpp>
#include <common/ring_buffer.hpp>
#include "generic_interrupt_handler.hpp"
#include <platform/i386/interrupt_parameters.hpp> // AFOX_TODO: PS2KeyboardDevice needs to be made general enough to work on i386 and also x86_64.

class PS2KeyboardDevice : public KeyboardDevice, public GenericInterruptHandler<i386::InterruptParameters> {
public:
    virtual ~PS2KeyboardDevice() noexcept;
    PS2KeyboardDevice();

    // Keyboard interrupt handler.
    // Implements the lower half of PS/2 keyboard driver. To be called when the
    // keyboard interrupt fires.
    using ParameterPackage = i386::InterruptParameters;
    void int_handler(const ParameterPackage& params) noexcept override;

    const char* keycode_name(Keycode key) override;
    KeyboardEvent get_event() override;
    
private:
    enum KeyboardDriverState {
        IDLE,
        PROCESSING_ESCAPE_CODE
    };
    
    KeycodeKeyState key_state_[KEYCODE_MAX];
    RingBuffer<KeyboardEvent, 32> events_;
    
    void clear_input();
    bool step_state_machine(KeyboardDriverState &state, KeyboardEvent &output); // TODO: Extract this state machine to its own class.
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_DRIVERS_PC_PS2_KEYBOARD_DEVICE_HPP
