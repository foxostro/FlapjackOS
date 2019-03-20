#ifndef FLAPJACKOS_KERNEL_INCLUDE_BACKTRACE_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_BACKTRACE_HPP

#include <unwind.h> // for _Unwind_* symbols

class StackWalker {
public:
    virtual ~StackWalker() = default;
    virtual void trace(void* ip) = 0;
};

void backtrace(StackWalker& stack_walker);

#endif // FLAPJACKOS_KERNEL_INCLUDE_BACKTRACE_HPP
