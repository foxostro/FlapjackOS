#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_INTERRUPT_DISPATCHER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_INTERRUPT_DISPATCHER_HPP

#include "hardware_interrupt_controller.hpp"
#include "interrupt_parameters.hpp"
#include <generic_interrupt_dispatcher.hpp>

namespace i386 {

using InterruptDispatcher = GenericInterruptDispatcher<HardwareInterruptController, InterruptParameters>;

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_INTERRUPT_DISPATCHER_HPP
