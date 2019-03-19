#include <backtrace.hpp>
#include <common/logger.hpp>

void backtrace(TextTerminal &term)
{
    TRACE("begin");
    term.printf("Back Trace:\n");
    enumerate_stack_frames([&](uintptr_t instruction_pointer){
        TRACE("[%p]", reinterpret_cast<void*>(instruction_pointer));
        term.printf("[%p]\n", reinterpret_cast<void*>(instruction_pointer));
    });
    TRACE("end");
    term.printf("\n");
}
