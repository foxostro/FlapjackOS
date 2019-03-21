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

void PanicInterruptHandler::int_handler(const InterruptParameters& params)
{
    // This is a lot of characters. We need to ensure this memory is allocated
    // in .data and specifically NOT on the stack. Do this by marking it as
    // static. We certainly don't need to worry about reentrancy here since
    // 1) we're going to panic, and 2) we are in an interrupt context.
    static char text[1024] = {0};

    if (error_code_present_) {
        snprintf(text, sizeof(text),
                "Registers:\n"
                "r15 = %p\tr14 = %p\n"
                "r13 = %p\tr12 = %p\n"
                "r11 = %p\tr10 = %p\n"
                "r9  = %p\tr8  = %p\n"
                "rdi = %p\trsi = %p\n"
                "rbp = %p\trsp = %p\n"
                "rbx = %p\trdx = %p\n"
                "rcx = %p\trax = %p\n"
                "rip = %p\n"
                "Error Code: 0x%x\n"
                "%s",
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.r15)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.r14)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.r13)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.r12)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.r11)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.r10)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.r9)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.r8)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.rdi)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.rsi)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.rbp)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.rsp)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.rbx)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.rdx)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.rcx)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.rax)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.rip)),
                static_cast<unsigned>(params.error_code),
                message_);
    } else {
        snprintf(text, sizeof(text),
                "Registers:\n"
                "r15 = %p\tr14 = %p\n"
                "r13 = %p\tr12 = %p\n"
                "r11 = %p\tr10 = %p\n"
                "r9  = %p\tr8  = %p\n"
                "rdi = %p\trsi = %p\n"
                "rbp = %p\trsp = %p\n"
                "rbx = %p\trdx = %p\n"
                "rcx = %p\trax = %p\n"
                "rip = %p\n"
                "%s",
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.r15)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.r14)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.r13)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.r12)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.r11)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.r10)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.r9)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.r8)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.rdi)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.rsi)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.rbp)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.rsp)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.rbx)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.rdx)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.rcx)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.rax)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.rip)),
                message_);
    }
    panic(text);
}

} // namespace x86_64
