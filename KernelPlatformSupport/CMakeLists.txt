cmake_minimum_required(VERSION 3.6)

set(INCLUDE_FILES
    "include/gdt_asm.h"
    "include/get_frame_pointer.h"
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
        "src/${CMAKE_SYSTEM_PROCESSOR}/gdt_asm.S"
        "src/${CMAKE_SYSTEM_PROCESSOR}/get_frame_pointer.S"
        "src/${CMAKE_SYSTEM_PROCESSOR}/halt.S"
        "src/${CMAKE_SYSTEM_PROCESSOR}/idt_asm.S"
        "src/${CMAKE_SYSTEM_PROCESSOR}/inout.S"
        "src/${CMAKE_SYSTEM_PROCESSOR}/interrupt_asm.S"
        "${CMAKE_CURRENT_BINARY_DIR}/isr_wrapper_asm.S"
        "src/${CMAKE_SYSTEM_PROCESSOR}/ltr.S"
        "src/${CMAKE_SYSTEM_PROCESSOR}/creg.S"
        )
    GenerateSourceFromTemplate("${PROJECT_SOURCE_DIR}/KernelPlatformSupport/src/${CMAKE_SYSTEM_PROCESSOR}/isr_wrapper_asm.S.erb"
                               "${CMAKE_CURRENT_BINARY_DIR}/isr_wrapper_asm.S")
else(CMAKE_CROSSCOMPILING)
    set(SOURCE_FILES
        "src/host/get_frame_pointer.c"
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

set(KERNEL_PLATFORM_SUPPORT_LIBRARIES KernelPlatformSupport PARENT_SCOPE)
set(KERNEL_PLATFORM_SUPPORT_LIBRARIES_DIR "${PROJECT_SOURCE_DIR}/KernelPlatformSupport" PARENT_SCOPE)
set(KERNEL_PLATFORM_SUPPORT_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/KernelPlatformSupport/include" PARENT_SCOPE)
