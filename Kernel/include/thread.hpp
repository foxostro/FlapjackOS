#ifndef FLAPJACKOS_KERNEL_INCLUDE_THREAD_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_THREAD_HPP

#include <page_size.hpp>
#include <common/static_stack.hpp>

class Thread {
public:
    virtual ~Thread() = default;
    virtual char* switch_to() = 0;
    virtual void switch_away(Thread& next) = 0;

    StaticStack<PAGE_SIZE> stack_;
};

// The current thread yields the remainder of its time slice to the next one.
extern "C" void yield();
#endif // FLAPJACKOS_KERNEL_INCLUDE_THREAD_HPP
