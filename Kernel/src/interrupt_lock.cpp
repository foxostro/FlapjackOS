#include <interrupt_lock.hpp>
#include <kernel.hpp>

void InterruptLock::lock()
{
    get_global_kernel().disable_interrupts();
}

void InterruptLock::unlock()
{
    get_global_kernel().enable_interrupts();
}
