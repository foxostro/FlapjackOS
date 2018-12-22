#include <platform/x86_64/panic_interrupt_handler.hpp>
#include <panic.h>
#include <common/minmax.hpp>
#include <cstring>
#include <cstdio>

namespace x86_64 {

PanicInterruptHandler::PanicInterruptHandler(const char *message, bool error_code_present)
 : error_code_present_(error_code_present)
{
    memset(message_, 0, sizeof(message_));
    memcpy(message_, message, MIN(sizeof(message_), strlen(message))-1);
}

void PanicInterruptHandler::int_handler(const InterruptParameters& params) noexcept
{
    // This is a lot of characters. We need to ensure this memory is allocated
    // in .data and specifically NOT on the stack. Do this by marking it as
    // static. We certainly don't need to worry about reentrancy here since
    // 1) we're going to panic, and 2) we are in an interrupt context.
    static char text[1024] = {0};

    if (error_code_present_) {
        snprintf(text, sizeof(text),
                "Registers:\n"
                "r15 = %p\n"
                "r14 = %p\n"
                "r13 = %p\n"
                "r12 = %p\n"
                "r11 = %p\n"
                "r10 = %p\n"
                "r9 = %p\n"
                "r8 = %p\n"
                "rdi = %p\n"
                "rsi = %p\n"
                "rbp = %p\n"
                "rsp = %p\n"
                "rbx = %p\n"
                "rdx = %p\n"
                "rcx = %p\n"
                "rax = %p\n"
                "rip = %p\n"
                "Error Code: 0x%x\n"
                "%s",
                (void*)(uintptr_t)params.r15,
                (void*)(uintptr_t)params.r14,
                (void*)(uintptr_t)params.r13,
                (void*)(uintptr_t)params.r12,
                (void*)(uintptr_t)params.r11,
                (void*)(uintptr_t)params.r10,
                (void*)(uintptr_t)params.r9,
                (void*)(uintptr_t)params.r8,
                (void*)(uintptr_t)params.rdi,
                (void*)(uintptr_t)params.rsi,
                (void*)(uintptr_t)params.rbp,
                (void*)(uintptr_t)params.rsp,
                (void*)(uintptr_t)params.rbx,
                (void*)(uintptr_t)params.rdx,
                (void*)(uintptr_t)params.rcx,
                (void*)(uintptr_t)params.rax,
                (void*)(uintptr_t)params.rip,
                (unsigned)params.error_code,
                message_);
    } else {
        snprintf(text, sizeof(text),
                "Registers:\n"
                "r15 = %p\n"
                "r14 = %p\n"
                "r13 = %p\n"
                "r12 = %p\n"
                "r11 = %p\n"
                "r10 = %p\n"
                "r9 = %p\n"
                "r8 = %p\n"
                "rdi = %p\n"
                "rsi = %p\n"
                "rbp = %p\n"
                "rsp = %p\n"
                "rbx = %p\n"
                "rdx = %p\n"
                "rcx = %p\n"
                "rax = %p\n"
                "rip = %p\n"
                "%s",
                (void*)(uintptr_t)params.r15,
                (void*)(uintptr_t)params.r14,
                (void*)(uintptr_t)params.r13,
                (void*)(uintptr_t)params.r12,
                (void*)(uintptr_t)params.r11,
                (void*)(uintptr_t)params.r10,
                (void*)(uintptr_t)params.r9,
                (void*)(uintptr_t)params.r8,
                (void*)(uintptr_t)params.rdi,
                (void*)(uintptr_t)params.rsi,
                (void*)(uintptr_t)params.rbp,
                (void*)(uintptr_t)params.rsp,
                (void*)(uintptr_t)params.rbx,
                (void*)(uintptr_t)params.rdx,
                (void*)(uintptr_t)params.rcx,
                (void*)(uintptr_t)params.rax,
                (void*)(uintptr_t)params.rip,
                message_);
    }
    panic(text);
}

} // namespace x86_64
