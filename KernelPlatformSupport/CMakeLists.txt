cmake_minimum_required(VERSION 3.6)

set(INCLUDE_FILES
    "include/gdt_asm.h"
    "include/get_ebp.h"
    "include/halt.h"
    "include/idt_asm.h"
    "include/inout.h"
    "include/interrupt_asm.h"
    "include/ltr.h"
    "include/seg.h"
    "include/creg.h"
    )

# By its nature, libKernelPlatformSupport is the pieces of the kernel which are
# literally untestable. When we're building for the host, e.g. when building
# unit tests, we end up replacing most of these functions with stubs.
if(CMAKE_CROSSCOMPILING)
    set(SOURCE_FILES
        "src/i386/gdt_asm.S"
        "src/i386/get_ebp.S"
        "src/i386/halt.S"
        "src/i386/idt_asm.S"
        "src/i386/inout.S"
        "src/i386/interrupt_asm.S"
        "${CMAKE_CURRENT_BINARY_DIR}/isr_wrapper_asm.S"
        "src/i386/ltr.S"
        "src/i386/creg.S"
        )
    GenerateSourceFromTemplate("${PROJECT_SOURCE_DIR}/KernelPlatformSupport/src/i386/isr_wrapper_asm.S.erb"
                               "${CMAKE_CURRENT_BINARY_DIR}/isr_wrapper_asm.S")
else(CMAKE_CROSSCOMPILING)
    set(SOURCE_FILES
        "src/host/get_ebp.c"
        "src/host/gdt.c"
        "src/host/halt.c"
        "src/host/idt.c"
        "src/host/inout.c"
        "src/host/interrupt.c"
        "${CMAKE_CURRENT_BINARY_DIR}/isr_wrapper.c"
        "src/host/ltr.c"
        )
    GenerateSourceFromTemplate("${PROJECT_SOURCE_DIR}/KernelPlatformSupport/src/host/isr_wrapper.c.erb"
                               "${CMAKE_CURRENT_BINARY_DIR}/isr_wrapper.c")
endif(CMAKE_CROSSCOMPILING)

include_directories("include")

add_library(KernelPlatformSupport STATIC ${SOURCE_FILES})