#include <interrupt_lock.hpp>
#include <kernel.hpp>

void InterruptLock::lock()
{
    g_kernel.disable_interrupts();
}

void InterruptLock::unlock()
{
    g_kernel.enable_interrupts();
}
