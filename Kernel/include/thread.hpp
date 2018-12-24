#ifndef FLAPJACKOS_KERNEL_INCLUDE_THREAD_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_THREAD_HPP

#include <page_size.hpp>
#include <common/static_stack.hpp>

class Thread {
public:
    virtual ~Thread() = default;
    virtual char* switch_to() = 0;
    virtual void context_switch(Thread& next) = 0;

    StaticStack<PAGE_SIZE> stack_;
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_THREAD_HPP
