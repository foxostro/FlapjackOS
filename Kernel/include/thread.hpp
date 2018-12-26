#ifndef FLAPJACKOS_KERNEL_INCLUDE_THREAD_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_THREAD_HPP

#include <page_size.hpp>
#include <interrupt_lock.hpp>
#include <common/static_stack.hpp>

class Thread {
public:
    using Lock = InterruptLock;
    virtual ~Thread() = default;
    virtual char* switch_to(Lock& lock) = 0;
    virtual void switch_away(Lock& lock, Thread& next) = 0;
    virtual char*& get_stack_pointer() = 0;
};

// The current thread yields the remainder of its time slice to the next one.
extern "C" void yield();

// The current thread exits.
extern "C" __attribute__((noreturn)) void vanish();

#endif // FLAPJACKOS_KERNEL_INCLUDE_THREAD_HPP
