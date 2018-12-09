cmake_minimum_required(VERSION 3.6)

# AFOX_TODO: Some of these platform-agnostic source files really are not platform-agnostic at all and assume an Intel CPU. Need to address that somehow.
set(PLATFORM_AGNOSTIC_SOURCE_FILES
    "include/get_frame_pointer.h"
    "include/halt.h"
    "include/idt_asm.h"
    "include/inout.h"
    "include/interrupt_asm.h"
    "include/seg.h"
    "include/page_size.hpp"
    "include/logical_addressing.hpp"
    "include/platform/i386/idt.h" "src/platform/i386/idt.c"
    )

set(I386_SOURCE_FILES
    "include/platform/i386/isr_install.h" "${CMAKE_CURRENT_BINARY_DIR}/i386_isr_install.c"
    "include/platform/i386/page_directory.hpp"
    "include/platform/i386/page_directory_entry.hpp"
    "include/platform/i386/page_table.hpp"
    "include/platform/i386/page_table_entry.hpp"
    "include/platform/i386/physical_memory_map.hpp" "src/platform/i386/physical_memory_map.cpp"
    "include/platform/i386/kernel_address_space_bootstrapper.hpp" "src/platform/i386/kernel_address_space_bootstrapper.cpp"
    "include/platform/i386/paging_resolver.hpp"
    "include/platform/i386/hardware_memory_management_unit.hpp"
    "include/platform/i386/hardware_task_configuration.hpp" "src/platform/i386/hardware_task_configuration.cpp"
    "include/platform/i386/gdt.hpp" "src/platform/i386/gdt.cpp"
    "include/platform/i386/tss.hpp"
    "include/platform/i386/hardware_interrupt_controller.hpp" "src/platform/i386/hardware_interrupt_controller.cpp"
    "include/platform/i386/lgdt.hpp" "src/platform/i386/lgdt.S"
    "include/platform/i386/ltr.hpp" "src/platform/i386/ltr.S"
    "include/platform/i386/creg.hpp" "src/platform/i386/creg.S"
    )

set(X86_64_SOURCE_FILES
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
    "include/platform/x86_64/hardware_interrupt_controller.hpp" "src/platform/x86_64/hardware_interrupt_controller.cpp"
    "include/platform/x86_64/lgdt.hpp" "src/platform/x86_64/lgdt.S"
    "include/platform/x86_64/ltr.hpp" "src/platform/x86_64/ltr.S"
    "include/platform/x86_64/creg.hpp" "src/platform/x86_64/creg.S"
    "include/platform/x86_64/kernel_address_space_bootstrapper.hpp" "src/platform/x86_64/kernel_address_space_bootstrapper.cpp"
    "include/platform/x86_64/paging_resolver.hpp"
    "include/platform/x86_64/hardware_memory_management_unit.hpp"
    )

set(PC_SOURCE_FILES
    "include/platform/pc/pic.h" "src/platform/pc/pic.c"
    )
    
if(CMAKE_CROSSCOMPILING)
    set(PLATFORM_SPECIFIC_SOURCE_FILES
        "src/${BUILD_ARCH}/get_frame_pointer.S"
        "src/${BUILD_ARCH}/halt.S"
        "src/${BUILD_ARCH}/idt_asm.S"
        "src/${BUILD_ARCH}/inout.S"
        "src/${BUILD_ARCH}/interrupt_asm.S"
        "${CMAKE_CURRENT_BINARY_DIR}/isr_wrapper_asm.S"
        )
    GenerateSourceFromTemplate("${PROJECT_SOURCE_DIR}/KernelPlatformSupport/src/${CMAKE_SYSTEM_PROCESSOR}/isr_wrapper_asm.S.erb"
                               "${CMAKE_CURRENT_BINARY_DIR}/isr_wrapper_asm.S")

    list(APPEND PLATFORM_SPECIFIC_SOURCE_FILES
        "${PC_SOURCE_FILES}"
        )
else(CMAKE_CROSSCOMPILING)
    set(PLATFORM_SPECIFIC_SOURCE_FILES
        "src/host/get_frame_pointer.c"
        "src/host/halt.c"
        "src/host/idt.c"
        "src/host/inout.c"
        "src/host/interrupt.c"
        "${CMAKE_CURRENT_BINARY_DIR}/isr_wrapper.c"
        "${PC_SOURCE_FILES}"
        )
    GenerateSourceFromTemplate("${PROJECT_SOURCE_DIR}/KernelPlatformSupport/src/host/isr_wrapper.c.erb"
                               "${CMAKE_CURRENT_BINARY_DIR}/isr_wrapper.c")
endif(CMAKE_CROSSCOMPILING)

if (${BUILD_ARCH} STREQUAL "i386")
    list(APPEND PLATFORM_SPECIFIC_SOURCE_FILES
         "${I386_SOURCE_FILES}"
         )
elseif (${BUILD_ARCH} STREQUAL "x86_64")
    list(APPEND PLATFORM_SPECIFIC_SOURCE_FILES
         "${X86_64_SOURCE_FILES}"
         )
endif ()

set(SOURCE_FILES
    "${PLATFORM_SPECIFIC_SOURCE_FILES}"
    "${PLATFORM_AGNOSTIC_SOURCE_FILES}"
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

    set(TEST_SOURCE_FILES
        "test/test_main.cpp"
        "test/test_idt.cpp"
        )

    if (BUILD_32_BIT_TESTS)
        list(APPEND TEST_SOURCE_FILES
             "test/test_i386_gdt.cpp"
             "test/test_i386_page_directory_entry.cpp"
             "test/test_i386_page_table_entry.cpp"
             )
    else (BUILD_32_BIT_TESTS)
        list(APPEND TEST_SOURCE_FILES
             "test/test_x86_64_gdt.cpp"
             "test/test_x86_64_tss.cpp"
             "test/test_x86_64_generic_page_directory_entry.cpp"
             "test/test_x86_64_page_table_entry.cpp"
             "test/test_x86_64_physical_memory_map.cpp"
             "test/test_x86_64_kernel_address_space_bootstrapper.cpp"
             )
    endif (BUILD_32_BIT_TESTS)

    add_executable(KernelPlatformSupportTest
                   "${TEST_SOURCE_FILES}"
                   )

    target_link_libraries(KernelPlatformSupportTest
                          KernelPlatformSupport
                          )

    add_test(KernelPlatformSupportTest ${CMAKE_CURRENT_BINARY_DIR}/KernelPlatformSupportTest)

endif (NOT CMAKE_CROSSCOMPILING)

set(KERNEL_PLATFORM_SUPPORT_LIBRARIES KernelPlatformSupport PARENT_SCOPE)
set(KERNEL_PLATFORM_SUPPORT_LIBRARIES_DIR "${PROJECT_SOURCE_DIR}/KernelPlatformSupport" PARENT_SCOPE)
set(KERNEL_PLATFORM_SUPPORT_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/KernelPlatformSupport/include" PARENT_SCOPE)
