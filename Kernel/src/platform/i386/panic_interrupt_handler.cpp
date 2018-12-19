#include <platform/i386/panic_interrupt_handler.hpp>
#include <panic.h>
#include <common/minmax.hpp>
#include <cstring>
#include <cstdio>

namespace i386 {

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
                "edi = 0x%x\n"
                "esi = 0x%x\n"
                "ebp = 0x%x\n"
                "esp = 0x%x\n"
                "ebx = 0x%x\n"
                "edx = 0x%x\n"
                "ecx = 0x%x\n"
                "eax = 0x%x\n"
                "eip = %p\n"
                "Error Code: 0x%x\n"
                "%s",
                params.edi,
                params.esi,
                params.ebp,
                params.esp,
                params.ebx,
                params.edx,
                params.ecx,
                params.eax,
                (void*)(uintptr_t)params.eip,
                params.error_code,
                message_);
    } else {
        snprintf(text, sizeof(text),
                "Registers:\n"
                "edi = 0x%x\n"
                "esi = 0x%x\n"
                "ebp = 0x%x\n"
                "esp = 0x%x\n"
                "ebx = 0x%x\n"
                "edx = 0x%x\n"
                "ecx = 0x%x\n"
                "eax = 0x%x\n"
                "eip = %p\n"
                "%s",
                params.edi,
                params.esi,
                params.ebp,
                params.esp,
                params.ebx,
                params.edx,
                params.ecx,
                params.eax,
                (void*)(uintptr_t)params.eip,
                message_);
    }
    panic(text);
}

} // namespace i386
