#include <platform/x86_64/hardware_interrupt_controller.hpp>
#include <cassert>

namespace x86_64 {

HardwareInterruptController::HardwareInterruptController() = default;

void HardwareInterruptController::init()
{
    assert(!"unimplemented");
}

void HardwareInterruptController::disable_interrupts()
{
    assert(!"unimplemented");
}

void HardwareInterruptController::enable_interrupts()
{
    assert(!"unimplemented");
}

bool HardwareInterruptController::clear(unsigned interrupt_number)
{
    assert(!"unimplemented");
    (void)interrupt_number;
    return false;
}

} // namespace x86_64
