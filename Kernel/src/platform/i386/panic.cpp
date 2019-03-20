#include <panic.h> // The panic API uses C linkage.

#include <kernel_policy.hpp>
#include <common/text_terminal.hpp>
#include <halt.h>
#include <interrupt_asm.h>
#include <unwind.h> // for _Unwind_* symbols

#include <cstdio>
#include <cstdarg>

static _Unwind_Reason_Code trace(struct _Unwind_Context* context, void* param);

class PanicKernel : private KernelPolicy {
public:
    virtual ~PanicKernel() = default;

    PanicKernel(const char* message)
     : message_(message),
       terminal_(display_)
    {
        // hardware_interrupt_controller_.init(/*panic=*/ true);
    }

    __attribute__((noreturn)) void run() noexcept
    {
        display_.clear();
        puts("PANIC: ");
        puts(message_);
        puts("\n");
        backtrace();
        halt_forever();
    }

    void puts(const char* s)
    {
        logger_.puts(s);
        terminal_.puts(s);
    }

    void backtrace()
    {
        puts("Back Trace:\n");
        _Unwind_Backtrace(::trace, reinterpret_cast<void*>(this));
        puts("End Back Trace\n");
    }

    _Unwind_Reason_Code trace(struct _Unwind_Context* context)
    {
        void* ip = reinterpret_cast<void*>(_Unwind_GetIP(context));
        if (ip) {
            char buffer[32] = {0};
            snprintf(buffer, sizeof(buffer), "[%p]\n",
                    reinterpret_cast<void*>(ip));
            puts(buffer);
            return _URC_NO_REASON;
        } else {
            return _URC_END_OF_STACK;
        }
    }

    __attribute__((noreturn)) void interrupt() noexcept
    {
        const char* s = "Interrupt occurred during panic. Halting immediately.";
        logger_.puts(s);
        terminal_.puts(s);
        halt_forever();
    }

protected:
    const char* message_;
    HardwareInterruptController hardware_interrupt_controller_;
    TextDisplayDevice display_;
    TextTerminal terminal_;
    LoggerTextOutputStream logger_;
};

static _Unwind_Reason_Code trace(struct _Unwind_Context* context, void* param)
{
    return reinterpret_cast<PanicKernel*>(param)->trace(context);
}

static PanicKernel* g_panic_kernel;

// Prints a message to the screen and halts forever.
// This is only called in emergencies.
// Further, panic() provides a platform-agnostic interface though it relies on
// a platform-dependent implementation. This implementation is suitable for
// either a i386 or x86_64 PC system. It takes control of the VGA text display
// device and prints a message at the top of the console.
extern "C"
__attribute__((noreturn))
void panic(const char* fmt, ...)
{
    disable_interrupts();

    constexpr size_t PANIC_BUFFER_SIZE = 1024;
    char buffer[PANIC_BUFFER_SIZE];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    PanicKernel panic_kernel{buffer};
    g_panic_kernel = &panic_kernel;
    panic_kernel.run();
}

// Call this when an interrupt or fault occurs during a panic.
__attribute__((noreturn)) void on_interrupt_during_panic() noexcept
{
    g_panic_kernel->interrupt();
}
