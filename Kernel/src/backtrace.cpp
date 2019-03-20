#include <backtrace.hpp>

static const char* get_symbol_name([[maybe_unused]] const void* ip)
{
    return nullptr;
}

static _Unwind_Reason_Code backtrace_trace(struct _Unwind_Context* context, void* param)
{
    const void* ip = reinterpret_cast<const void*>(_Unwind_GetIP(context));
    if (ip) {
        StackWalker::StackFrame frame;
        frame.instruction_pointer = ip;
        frame.symbol_name = get_symbol_name(ip);
        reinterpret_cast<StackWalker*>(param)->trace(frame);
        return _URC_NO_REASON;
    } else {
        return _URC_END_OF_STACK;
    }
}

void backtrace(StackWalker& stack_walker)
{
    _Unwind_Backtrace(backtrace_trace, reinterpret_cast<void*>(&stack_walker));
}
