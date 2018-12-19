#ifndef FLAPJACKOS_KERNEL_INCLUDE_SIMPLE_DEVICE_INTERRUPT_HANDLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_SIMPLE_DEVICE_INTERRUPT_HANDLER_HPP

#include "generic_interrupt_handler.hpp"

template<typename InterruptParameters, typename Device>
class SimpleDeviceInterruptHandler : public GenericInterruptHandler<InterruptParameters> {
public:
    SimpleDeviceInterruptHandler(Device& device)
     : device_(device)
    {}

    void int_handler(const InterruptParameters&) noexcept override
    {
        device_.on_interrupt();
    }

private:
    Device& device_;
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_SIMPLE_DEVICE_INTERRUPT_HANDLER_HPP
