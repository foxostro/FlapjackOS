cmake_minimum_required(VERSION 3.6)

set(SOURCE_FILES
    "include/misc.h"
    "include/tss.h"
    "include/idt.h" "src/idt.c"
    "include/gdt.h" "src/gdt.c"
    "include/isr_install.h" "${CMAKE_CURRENT_BINARY_DIR}/isr_install.c"
    "include/backtrace.hpp" "src/backtrace.cpp"
    "include/panic.h" "src/panic.cpp"
    "include/pic.h" "src/pic.c"
    "include/timer_device.hpp"
    "include/pit_timer_device.hpp" "src/pit_timer_device.cpp"
    "include/ps2_keyboard_device.hpp" "src/ps2_keyboard_device/ps2_keyboard_device.cpp"
    "include/vga_text_display_device.hpp" "src/vga_text_display_device.cpp"
    "src/ps2_keyboard_device/keyboard_scancodes_make_escaped.inc"
    "src/ps2_keyboard_device/keyboard_scancodes_break.inc"
    "src/ps2_keyboard_device/keyboard_keycode_ascii_uppercase.inc"
    "src/ps2_keyboard_device/keyboard_keycode_names.inc"
    "src/ps2_keyboard_device/keyboard_keycode_ascii_lowercase.inc"
    "src/ps2_keyboard_device/keyboard_scancodes_break_escaped.inc"
    "src/ps2_keyboard_device/keyboard_scancodes_make.inc"
    "include/kernel.hpp" "src/kernel.cpp"
    "include/page_size.hpp"
    "include/kernel_image_info.hpp"
    "include/kernel_contiguous_memory_allocator.hpp" "src/kernel_contiguous_memory_allocator.cpp"
    "include/kernel_break_allocator.hpp" "src/kernel_break_allocator.cpp"
    "include/mmu.hpp" "src/mmu.cpp"
    "include/page_frame_allocator.hpp" "src/page_frame_allocator.cpp"
    "include/page_frame_allocator_factory.hpp" "src/page_frame_allocator_factory.cpp"
    "include/kernel_page_directory_populate_operation.hpp" "src/kernel_page_directory_populate_operation.cpp"
    "include/cleanup_kernel_memory_map_operation.hpp" "src/cleanup_kernel_memory_map_operation.cpp"
    "include/kernel_memory_allocators.hpp" "src/kernel_memory_allocators.cpp"
    "include/interrupt_dispatcher.hpp" "src/interrupt_dispatcher.cpp"
    "include/interrupt_lock.hpp" "src/interrupt_lock.cpp"
    "include/panic_interrupt_handler.hpp" "src/panic_interrupt_handler.cpp"
    "include/page_fault_interrupt_handler.hpp" "src/page_fault_interrupt_handler.cpp"
    )

GenerateSourceFromTemplate("${PROJECT_SOURCE_DIR}/Kernel/src/isr_install.c.erb"
                           "${CMAKE_CURRENT_BINARY_DIR}/isr_install.c")

include_directories("include"
                    ${COMMON_INCLUDE_DIRS}
                    ${MALLOC_INCLUDE_DIRS}
                    ${KERNEL_PLATFORM_SUPPORT_INCLUDE_DIRS}
                    )

link_directories(${COMMON_LIBRARIES_DIR}
                 ${MALLOC_LIBRARIES_DIRS}
                 ${KERNEL_PLATFORM_SUPPORT_LIBRARIES_DIR}
                 )

add_library(Kernel STATIC ${SOURCE_FILES})

target_link_libraries(Kernel
                      ${COMMON_LIBRARIES}
                      ${MALLOC_LIBRARIES}
                      ${KERNEL_PLATFORM_SUPPORT_LIBRARIES}
                      )

if (NOT CMAKE_CROSSCOMPILING)

    add_executable(KernelTest
                   "test/test_gdt.cpp"
                   "test/test_idt.cpp"
                   "test/test_main.cpp"
                   )

    target_link_libraries(KernelTest
                          Kernel
                          )

    add_test(KernelTest ${CMAKE_CURRENT_BINARY_DIR}/KernelTest)

endif (NOT CMAKE_CROSSCOMPILING)
