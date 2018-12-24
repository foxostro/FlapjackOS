cmake_minimum_required(VERSION 3.6)

# AFOX_TODO: Some of these platform-agnostic source files really are not
# platform-agnostic and assume an Intel CPU or a PC system. Need to address
# that somehow...
set(PLATFORM_AGNOSTIC_SOURCE_FILES
    "include/backtrace.hpp" "src/backtrace.cpp"
    "include/panic.h" "src/panic.cpp"
    "include/kernel_policy.hpp" 
    "include/kernel.hpp" "src/kernel.cpp" "src/kernel_main.cpp"
    "include/thread.hpp"
    "include/page_frame_allocator.hpp"
    "include/page_frame_allocator_configuration_operation.hpp"
    "include/multiboot_memory_map_page_frame_enumerator.hpp"
    "include/generic_interrupt_dispatcher.hpp"
    "include/interrupt_lock.hpp" "src/interrupt_lock.cpp"
    "include/generic_interrupt_handler.hpp"
    "include/simple_device_interrupt_handler.hpp"
    "include/get_frame_pointer.h"
    "include/halt.h"
    "include/inout.h"
    "include/interrupt_asm.h"
    "include/seg.h"
    "include/page_size.hpp"
    "include/logical_addressing.hpp"
    "include/logger.hpp" "src/logger.cpp"
    "include/drivers/pc/ps2_keyboard_device.hpp" "src/drivers/pc/ps2_keyboard_device/ps2_keyboard_device.cpp"
    "src/drivers/pc/ps2_keyboard_device/keyboard_scancodes_make_escaped.inc"
    "src/drivers/pc/ps2_keyboard_device/keyboard_scancodes_break.inc"
    "src/drivers/pc/ps2_keyboard_device/keyboard_keycode_ascii_uppercase.inc"
    "src/drivers/pc/ps2_keyboard_device/keyboard_keycode_names.inc"
    "src/drivers/pc/ps2_keyboard_device/keyboard_keycode_ascii_lowercase.inc"
    "src/drivers/pc/ps2_keyboard_device/keyboard_scancodes_break_escaped.inc"
    "src/drivers/pc/ps2_keyboard_device/keyboard_scancodes_make.inc"
    "include/drivers/pc/vga_text_display_device.hpp" "src/drivers/pc/vga_text_display_device.cpp"
    "include/drivers/pc/pit_timer_device.hpp" "src/drivers/pc/pit_timer_device.cpp"
    )

set(I386_SOURCE_FILES
    "include/platform/i386/page_directory.hpp"
    "include/platform/i386/page_directory_entry.hpp"
    "include/platform/i386/page_table.hpp"
    "include/platform/i386/page_table_entry.hpp"
    "include/platform/i386/generic_physical_memory_map.hpp"
    "include/platform/i386/physical_memory_map.hpp"
    "include/platform/i386/generic_kernel_address_space_bootstrapper.hpp"
    "include/platform/i386/kernel_address_space_bootstrapper.hpp"
    "include/platform/i386/paging_resolver.hpp"
    "include/platform/i386/hardware_memory_management_unit.hpp"
    "include/platform/i386/hardware_task_configuration.hpp" "src/platform/i386/hardware_task_configuration.cpp"
    "include/platform/i386/gdt.hpp" "src/platform/i386/gdt.cpp"
    "include/platform/i386/tss.hpp"
    "include/platform/i386/interrupt_controller.hpp"
    "include/platform/i386/interrupt_parameters.hpp"
    "include/platform/i386/interrupt_dispatcher.hpp"
	"include/platform/i386/panic_interrupt_handler.hpp" "src/platform/i386/panic_interrupt_handler.cpp"
    "include/platform/i386/page_fault_interrupt_handler.hpp" "src/platform/i386/page_fault_interrupt_handler.cpp"
    "include/platform/i386/hardware_interrupt_controller.hpp"
    "include/platform/i386/device_drivers.hpp"
    "include/platform/i386/lgdt.hpp" "src/platform/i386/lgdt.S"
    "include/platform/i386/ltr.hpp" "src/platform/i386/ltr.S"
    "include/platform/i386/creg.hpp" "src/platform/i386/creg.S"
    "include/platform/i386/interrupt_numbers.hpp"
    "include/platform/i386/lidt.hpp" "src/platform/i386/lidt.S"
    "include/platform/i386/idt.hpp"
    "include/platform/i386/interrupt_descriptor.hpp"
    "include/platform/i386/context_switch.hpp" "src/platform/i386/context_switch.S"
    "include/platform/i386/thread.hpp"
    )

set(X86_64_SOURCE_FILES
    "include/platform/x86_64/hardware_task_configuration.hpp" "src/platform/x86_64/hardware_task_configuration.cpp"
    "include/platform/x86_64/tss.hpp"
    "include/platform/x86_64/gdt.hpp" "src/platform/x86_64/gdt.cpp"
    "include/platform/x86_64/physical_memory_map.hpp"
    "include/platform/x86_64/page_directory_pointer_table.hpp"
    "include/platform/x86_64/page_table_entry.hpp"
    "include/platform/x86_64/generic_page_directory_entry.hpp"
    "include/platform/x86_64/page_directory_pointer_table_entry.hpp"
    "include/platform/x86_64/page_map_level_four.hpp"
    "include/platform/x86_64/page_directory.hpp"
    "include/platform/x86_64/page_map_level_four_entry.hpp"
    "include/platform/x86_64/page_directory_entry.hpp"
    "include/platform/x86_64/page_table.hpp"
    "include/platform/x86_64/interrupt_controller.hpp"
    "include/platform/x86_64/interrupt_parameters.hpp"
    "include/platform/x86_64/interrupt_dispatcher.hpp"
	"include/platform/x86_64/panic_interrupt_handler.hpp" "src/platform/x86_64/panic_interrupt_handler.cpp"
    "include/platform/x86_64/page_fault_interrupt_handler.hpp" "src/platform/x86_64/page_fault_interrupt_handler.cpp"
    "include/platform/x86_64/hardware_interrupt_controller.hpp"
    "include/platform/x86_64/device_drivers.hpp"
    "include/platform/x86_64/lgdt.hpp" "src/platform/x86_64/lgdt.S"
    "include/platform/x86_64/ltr.hpp" "src/platform/x86_64/ltr.S"
    "include/platform/x86_64/creg.hpp" "src/platform/x86_64/creg.S"
    "include/platform/i386/generic_kernel_address_space_bootstrapper.hpp"
    "include/platform/x86_64/kernel_address_space_bootstrapper.hpp"
    "include/platform/x86_64/paging_resolver.hpp"
    "include/platform/x86_64/hardware_memory_management_unit.hpp"
    "include/platform/i386/interrupt_numbers.hpp"
    "include/platform/x86_64/lidt.hpp" "src/platform/x86_64/lidt.S"
    "include/platform/x86_64/idt.hpp"
    "include/platform/x86_64/interrupt_descriptor.hpp"
    "include/platform/x86_64/context_switch.hpp" "src/platform/x86_64/context_switch.S"
    "include/platform/x86_64/thread.hpp"
    )

set(PC_SOURCE_FILES
    "include/platform/pc/pic.hpp" "src/platform/pc/pic.cpp"
    )

set(PLATFORM_SPECIFIC_SOURCE_FILES "${PC_SOURCE_FILES}")

if(CMAKE_CROSSCOMPILING)
    GenerateSourceFromTemplate("${PROJECT_SOURCE_DIR}/Kernel/src/${CMAKE_SYSTEM_PROCESSOR}/isr_wrapper_asm.S.erb"
                               "${CMAKE_CURRENT_BINARY_DIR}/isr_wrapper_asm.S")
    list(APPEND PLATFORM_SPECIFIC_SOURCE_FILES
         "src/${BUILD_ARCH}/get_frame_pointer.S"
         "src/${BUILD_ARCH}/halt.S"
         "src/${BUILD_ARCH}/inout.S"
         "src/${BUILD_ARCH}/interrupt_asm.S"
         "${CMAKE_CURRENT_BINARY_DIR}/isr_wrapper_asm.S"
         )
else(CMAKE_CROSSCOMPILING)
    GenerateSourceFromTemplate("${PROJECT_SOURCE_DIR}/Kernel/src/host/isr_wrapper.c.erb"
                               "${CMAKE_CURRENT_BINARY_DIR}/isr_wrapper.c")
    list(APPEND PLATFORM_SPECIFIC_SOURCE_FILES
         "src/host/get_frame_pointer.c"
         "src/host/halt.c"
         "src/host/inout.c"
         "src/host/interrupt.c"
         "${CMAKE_CURRENT_BINARY_DIR}/isr_wrapper.c"
         )
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

include_directories("include"
                    ${COMMON_INCLUDE_DIRS}
                    ${MALLOC_INCLUDE_DIRS}
                    ${BOOT_INCLUDE_DIR}
                    )

link_directories(${COMMON_LIBRARIES_DIR}
                 ${MALLOC_LIBRARIES_DIRS}
                 )

add_library(Kernel STATIC ${SOURCE_FILES})

target_link_libraries(Kernel
                      ${COMMON_LIBRARIES}
                      ${MALLOC_LIBRARIES}
                      )

if (NOT CMAKE_CROSSCOMPILING)

	set(TEST_SOURCE_FILES
        "test/test_main.cpp"
        "test/test_page_frame_allocator.cpp"
        "test/test_page_frame_allocator_configuration_operation.cpp"
        )

    if (${BUILD_ARCH} STREQUAL "i386")
        list(APPEND TEST_SOURCE_FILES
             "test/test_i386_gdt.cpp"
             "test/test_i386_idt.cpp"
             "test/test_i386_page_directory_entry.cpp"
             "test/test_i386_page_table_entry.cpp"
             "test/test_i386_physical_memory_map.cpp"
             "test/test_i386_kernel_address_space_bootstrapper.cpp"
             )
    elseif (${BUILD_ARCH} STREQUAL "x86_64")
        list(APPEND TEST_SOURCE_FILES
             "test/test_x86_64_gdt.cpp"
             "test/test_x86_64_idt.cpp"
             "test/test_x86_64_tss.cpp"
             "test/test_x86_64_generic_page_directory_entry.cpp"
             "test/test_x86_64_page_table_entry.cpp"
             "test/test_x86_64_physical_memory_map.cpp"
             "test/test_x86_64_kernel_address_space_bootstrapper.cpp"
             )
    endif ()


    add_executable(KernelTest
                   "${TEST_SOURCE_FILES}"
                   )

    target_link_libraries(KernelTest
                          Kernel
                          )

    add_test(KernelTest ${CMAKE_CURRENT_BINARY_DIR}/KernelTest)

endif (NOT CMAKE_CROSSCOMPILING)

set(KERNEL_LIBRARIES Kernel PARENT_SCOPE)
set(KERNEL_LIBRARIES_DIR "${PROJECT_SOURCE_DIR}/Kernel" PARENT_SCOPE)
set(KERNEL_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/Kernel/include" PARENT_SCOPE)
