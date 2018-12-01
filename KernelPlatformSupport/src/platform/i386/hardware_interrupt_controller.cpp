#include <platform/i386/hardware_interrupt_controller.hpp>
#include <platform/i386/isr_install.h>
#include <platform/pc/pic.h>
#include <interrupt_asm.h>

namespace i386 {

HardwareInterruptController::HardwareInterruptController() = default;

void HardwareInterruptController::init()
{
    isr_install(idt_);
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

} // namespace i386
