#include <platform/i386/page_fault_interrupt_handler.hpp>
#include <platform/i386/creg.hpp>
#include <cstdio>
#include <panic.h>

namespace i386 {

// Returns a string interpretation of a page fault error code.
static const char* get_page_fault_error_string(unsigned error_code)
{
    switch (error_code) {
        case 0b000: return "supervisory process tried to read a non-present page entry";
        case 0b001: return "supervisory process tried to read a page and caused a protection fault";
        case 0b010: return "supervisory process tried to write to a non-present page entry";
        case 0b011: return "supervisory process tried to write a page and caused a protection fault";
        case 0b100: return "user process tried to read a non-present page entry";
        case 0b101: return "user process tried to read a page and caused a protection fault";
        case 0b110: return "user process tried to write to a non-present page entry";
        case 0b111: return "user process tried to write a page and caused a protection fault";
        default: return "unknown";
    }
}

PageFaultInterruptHandler::PageFaultInterruptHandler() = default;

void PageFaultInterruptHandler::int_handler(const InterruptParameters& params)
{
    char message[512]={0};
    snprintf(message, sizeof(message),
             "Page Fault. Faulting Address: %p\n"
             "Registers:\n"
             "edi = %p\tesi = %p\tebp = %p\n"
             "esp = %p\tebx = %p\tedx = %p\n"
             "ecx = %p\teax = %p\teip = %p\n"
             "Error Code: 0x%x\n"
             "%s",
             reinterpret_cast<void*>(static_cast<uintptr_t>(get_cr2())),
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
             get_page_fault_error_string(params.error_code));
    panic(message);
}

} // namespace i386
