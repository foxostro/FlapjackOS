#include <platform/x86_64/hardware_interrupt_controller.hpp>
#include <platform/pc/pic.h>
#include <interrupt_asm.h>

namespace x86_64 {

HardwareInterruptController::HardwareInterruptController() = default;

void HardwareInterruptController::init()
{
    pic_init();
}

void HardwareInterruptController::disable_interrupts()
{
    ::disable_interrupts();
}

void HardwareInterruptController::enable_interrupts()
{
    ::enable_interrupts();
}

bool HardwareInterruptController::clear(unsigned interrupt_number)
{
    return pic_clear(interrupt_number);
}

} // namespace x86_64
