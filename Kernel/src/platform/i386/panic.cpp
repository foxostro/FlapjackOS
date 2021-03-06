#include <panic.h> // The panic API uses C linkage.

#include <kernel_policy.hpp>
#include <common/text_terminal.hpp>
#include <halt.h>
#include <interrupt_asm.h>
#include <backtrace.hpp>

#include <cstdio>
#include <cstdarg>

class PanicKernel : public StackWalker, private KernelPolicy {
public:
    virtual ~PanicKernel() = default;

    PanicKernel(const char* message)
     : message_(message),
       terminal_(display_)
    {
        // AFOX_TODO: I want to replace the IDT during a panic. Unfortunately, this is a little messy and actually crashes on i386. Need to find a better solution.
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
        ::backtrace(*this);
        puts("End Back Trace\n");
    }

    void trace(const StackFrame& frame) override
    {
        char buffer[75] = {0};
        snprintf(buffer, sizeof(buffer), "%p: %s\n",
                 frame.instruction_pointer,
                 frame.symbol_name);
        puts(buffer);
    }

    __attribute__((noreturn)) void reentry(const char* message) noexcept
    {
        char buffer[160] = {0};
        snprintf(buffer, sizeof(buffer),
                 "Reentrant call to panic. "
                 "Halting immediately. "
                 "Message is:\n%s",
                 message);
        logger_.puts(buffer);
        terminal_.puts(buffer);
        halt_forever();
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

    constexpr size_t PANIC_BUFFER_SIZE = 512;
    char buffer[PANIC_BUFFER_SIZE];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    if (g_panic_kernel) {
        g_panic_kernel->reentry(buffer);
    }

    PanicKernel panic_kernel{buffer};
    g_panic_kernel = &panic_kernel;
    panic_kernel.run();
}

// Call this when an interrupt or fault occurs during a panic.
__attribute__((noreturn)) void on_interrupt_during_panic() noexcept
{
    g_panic_kernel->interrupt();
}
