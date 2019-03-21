#ifndef FLAPJACKOS_KERNEL_INCLUDE_BACKTRACE_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_BACKTRACE_HPP

#include <unwind.h> // for _Unwind_* symbols

class StackWalker {
public:
    struct StackFrame {
        const void* instruction_pointer = nullptr;
        const char* symbol_name = nullptr;
    };

    virtual ~StackWalker() = default;
    virtual void trace(const StackFrame& frame) = 0;
};

void backtrace(StackWalker& stack_walker);

#endif // FLAPJACKOS_KERNEL_INCLUDE_BACKTRACE_HPP
