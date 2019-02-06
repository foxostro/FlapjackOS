#include <platform/x86_64/page_fault_interrupt_handler.hpp>
#include <platform/x86_64/creg.hpp>
#include <cstdio>
#include <panic.h>

namespace x86_64 {

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
             reinterpret_cast<void*>(static_cast<uintptr_t>(get_cr2())),
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
             get_page_fault_error_string(params.error_code));
    panic(message);
}

} // namespace x86_64
