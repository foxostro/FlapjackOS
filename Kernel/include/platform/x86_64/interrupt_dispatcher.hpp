#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_INTERRUPT_DISPATCHER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_INTERRUPT_DISPATCHER_HPP

#include "hardware_interrupt_controller.hpp"
#include "interrupt_parameters.hpp"
#include <generic_interrupt_dispatcher.hpp>

namespace x86_64 {

using InterruptDispatcher = GenericInterruptDispatcher<HardwareInterruptController, InterruptParameters>;

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_INTERRUPT_DISPATCHER_HPP
