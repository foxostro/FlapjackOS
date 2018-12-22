#include <interrupt_lock.hpp>
#include <interrupt_asm.h>

// The GCC freestanding implementation does not seem to provide support for
// std::atomic. If it did then the counter below could be replaced with the
// std::atomic<int>. This change would enable InterruptLock to work correctly
// when we have preemption working in the kernel.
// AFOX_TODO: Use an atomic counter so that InterruptLock will work with a preemptive kernel.
static int g_interrupt_lock_count = 0;

void InterruptLock::lock()
{
    if (g_interrupt_lock_count++ == 0) {
        ::disable_interrupts();
    }
}

void InterruptLock::unlock()
{
    if (g_interrupt_lock_count-- == 0) {
        ::enable_interrupts();
    }
}
