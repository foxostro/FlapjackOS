cmake_minimum_required(VERSION 3.6)

# libKernelPlatformSupport contains some pieces which cannot be tested. These
# generally require a compile-time or link-time seam and involve platform-
# specific assembly code. When we're building for unit testing, we replace
# these functions with stubs.
if(CMAKE_CROSSCOMPILING)
    set(SOURCE_FILES
        "src/${CMAKE_SYSTEM_PROCESSOR}/get_frame_pointer.S"
        "src/${CMAKE_SYSTEM_PROCESSOR}/halt.S"
        "src/${CMAKE_SYSTEM_PROCESSOR}/idt_asm.S"
        "src/${CMAKE_SYSTEM_PROCESSOR}/inout.S"
        "src/${CMAKE_SYSTEM_PROCESSOR}/interrupt_asm.S"
        "${CMAKE_CURRENT_BINARY_DIR}/isr_wrapper_asm.S"
        )
    GenerateSourceFromTemplate("${PROJECT_SOURCE_DIR}/KernelPlatformSupport/src/${CMAKE_SYSTEM_PROCESSOR}/isr_wrapper_asm.S.erb"
                               "${CMAKE_CURRENT_BINARY_DIR}/isr_wrapper_asm.S")
else(CMAKE_CROSSCOMPILING)
    set(SOURCE_FILES
        "src/host/get_frame_pointer.c"
        "src/host/halt.c"
        "src/host/idt.c"
        "src/host/inout.c"
        "src/host/interrupt.c"
        "${CMAKE_CURRENT_BINARY_DIR}/isr_wrapper.c"
        )
    GenerateSourceFromTemplate("${PROJECT_SOURCE_DIR}/KernelPlatformSupport/src/host/isr_wrapper.c.erb"
                               "${CMAKE_CURRENT_BINARY_DIR}/isr_wrapper.c")
endif(CMAKE_CROSSCOMPILING)

list(APPEND SOURCE_FILES
     "include/get_frame_pointer.h"
     "include/halt.h"
     "include/idt_asm.h"
     "include/inout.h"
     "include/interrupt_asm.h"
     "include/seg.h"
     "include/page_size.hpp"
     "include/logical_addressing.hpp"

     "include/platform/i386/idt.h" "src/platform/i386/idt.c"
     "include/platform/i386/isr_install.h" "${CMAKE_CURRENT_BINARY_DIR}/i386_isr_install.c"
     "include/platform/i386/protection_flags.hpp"
     "include/platform/i386/page_directory.hpp" "src/platform/i386/page_directory.cpp"
     "include/platform/i386/page_directory_entry.hpp" "src/platform/i386/page_directory_entry.cpp"
     "include/platform/i386/page_table.hpp" "src/platform/i386/page_table.cpp"
     "include/platform/i386/page_table_entry.hpp" "src/platform/i386/page_table_entry.cpp"
     "include/platform/i386/physical_memory_map.hpp" "src/platform/i386/physical_memory_map.cpp"
     "include/platform/i386/mmu.hpp" "src/platform/i386/mmu.cpp"
     "include/platform/i386/kernel_address_space_bootstrapper.hpp" "src/platform/i386/kernel_address_space_bootstrapper.cpp"
     "include/platform/i386/hardware_task_configuration.hpp" "src/platform/i386/hardware_task_configuration.cpp"
     "include/platform/i386/gdt.hpp" "src/platform/i386/gdt.cpp"
     "include/platform/i386/tss.hpp"
     "include/platform/i386/hardware_interrupt_controller.hpp" "src/platform/i386/hardware_interrupt_controller.cpp"
     "include/platform/i386/lgdt.hpp" "src/platform/i386/lgdt.S"
     "include/platform/i386/ltr.hpp" "src/platform/i386/ltr.S"
     "include/platform/i386/creg.hpp" "src/platform/i386/creg.S"

     "include/platform/x86_64/hardware_task_configuration.hpp" "src/platform/x86_64/hardware_task_configuration.cpp"
     "include/platform/x86_64/tss.hpp"
     "include/platform/x86_64/gdt.hpp" "src/platform/x86_64/gdt.cpp"
     "include/platform/x86_64/physical_memory_map.hpp" "src/platform/x86_64/physical_memory_map.cpp"
     "include/platform/x86_64/page_directory_pointer_table.hpp"
     "include/platform/x86_64/page_table_entry.hpp"
     "include/platform/x86_64/generic_page_directory_entry.hpp"
     "include/platform/x86_64/page_directory_pointer_table_entry.hpp"
     "include/platform/x86_64/page_map_level_four.hpp"
     "include/platform/x86_64/page_directory.hpp"
     "include/platform/x86_64/page_map_level_four_entry.hpp"
     "include/platform/x86_64/page_directory_entry.hpp"
     "include/platform/x86_64/page_table.hpp"
     "include/platform/x86_64/lgdt.hpp" "src/platform/x86_64/lgdt.S"
     "include/platform/x86_64/ltr.hpp" "src/platform/x86_64/ltr.S"
     "include/platform/x86_64/creg.hpp" "src/platform/x86_64/creg.S"

     "include/platform/pc/pic.h" "src/platform/pc/pic.c"
     )

GenerateSourceFromTemplate("${PROJECT_SOURCE_DIR}/KernelPlatformSupport/src/platform/i386/isr_install.c.erb"
                           "${CMAKE_CURRENT_BINARY_DIR}/i386_isr_install.c")

include_directories("include"
                    ${COMMON_INCLUDE_DIRS}
                    ${BOOT_INCLUDE_DIR}
                    )

link_directories(${COMMON_LIBRARIES_DIR}
                 )

add_library(KernelPlatformSupport STATIC ${SOURCE_FILES})

target_link_libraries(KernelPlatformSupport
                      ${COMMON_LIBRARIES}
                      )

if (NOT CMAKE_CROSSCOMPILING)

    add_executable(KernelPlatformSupportTest
                   "test/test_idt.cpp"
                   "test/test_i386_gdt.cpp"
                   "test/test_i386_page_directory_entry.cpp"
                   "test/test_i386_page_table_entry.cpp"
                   "test/test_i386_page_directory.cpp"
                   "test/test_i386_page_table.cpp"
                   "test/test_x86_64_gdt.cpp"
                   "test/test_x86_64_tss.cpp"
                   "test/test_x86_64_generic_page_directory_entry.cpp"
                   "test/test_x86_64_page_table_entry.cpp"
                   "test/test_x86_64_physical_memory_map.cpp"
                   "test/test_main.cpp"
                   )

    target_link_libraries(KernelPlatformSupportTest
                          KernelPlatformSupport
                          )

    add_test(KernelPlatformSupportTest ${CMAKE_CURRENT_BINARY_DIR}/KernelPlatformSupportTest)

endif (NOT CMAKE_CROSSCOMPILING)

set(KERNEL_PLATFORM_SUPPORT_LIBRARIES KernelPlatformSupport PARENT_SCOPE)
set(KERNEL_PLATFORM_SUPPORT_LIBRARIES_DIR "${PROJECT_SOURCE_DIR}/KernelPlatformSupport" PARENT_SCOPE)
set(KERNEL_PLATFORM_SUPPORT_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/KernelPlatformSupport/include" PARENT_SCOPE)
