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
                "edi = 0x%x\tesi = 0x%x\tebp = 0x%x\n"
                "esp = 0x%x\tebx = 0x%x\tedx = 0x%x\n"
                "ecx = 0x%x\teax = 0x%x\teip = %p\n"
                "Error Code: 0x%x\n"
                "%s",
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.edi)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.esi)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.ebp)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.esp)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.ebx)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.edx)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.ecx)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.eax)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.eip)),
                static_cast<unsigned>(params.error_code),
                message_);
    } else {
        snprintf(text, sizeof(text),
                "Registers:\n"
                "edi = 0x%x\tesi = 0x%x\tebp = 0x%x\n"
                "esp = 0x%x\tebx = 0x%x\tedx = 0x%x\n"
                "ecx = 0x%x\teax = 0x%x\teip = %p\n"
                "%s",
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.edi)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.esi)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.ebp)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.esp)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.ebx)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.edx)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.ecx)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.eax)),
                reinterpret_cast<void*>(static_cast<uintptr_t>(params.eip)),
                message_);
    }
    panic(text);
}

} // namespace i386
