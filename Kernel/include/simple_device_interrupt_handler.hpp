#ifndef FLAPJACKOS_KERNEL_INCLUDE_SIMPLE_DEVICE_INTERRUPT_HANDLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_SIMPLE_DEVICE_INTERRUPT_HANDLER_HPP

#include "generic_interrupt_handler.hpp"

extern "C" {
    void yield(void) noexcept;
}

template<typename InterruptParameters, typename Device>
class SimpleDeviceInterruptHandler : public GenericInterruptHandler<InterruptParameters> {
public:
    SimpleDeviceInterruptHandler(Device& device)
     : device_(device)
    {}

    void int_handler(const InterruptParameters&) override
    {
        device_.on_interrupt();
        yield();
    }

private:
    Device& device_;
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_SIMPLE_DEVICE_INTERRUPT_HANDLER_HPP
