#include <interrupt_lock.hpp>
#include <interrupt_asm.h>
#include <logger.hpp>

// The GCC freestanding implementation does not seem to provide support for
// std::atomic. So, we use the GCC atomic built-ins instead.
static int g_interrupt_lock_count = 0;

void InterruptLock::lock()
{
    if (__atomic_fetch_add(&g_interrupt_lock_count, 1, __ATOMIC_SEQ_CST) == 0) {
        ::disable_interrupts();
    }
}

void InterruptLock::unlock()
{
    if (__atomic_fetch_sub(&g_interrupt_lock_count, 1, __ATOMIC_SEQ_CST) == 1) {
        ::enable_interrupts();
    }
}
