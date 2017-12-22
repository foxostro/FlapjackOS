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
    "include/vga_console_device.hpp" "src/vga_console_device.cpp"
    "src/ps2_keyboard_device/keyboard_scancodes_make_escaped.inc"
    "src/ps2_keyboard_device/keyboard_scancodes_break.inc"
    "src/ps2_keyboard_device/keyboard_keycode_ascii_uppercase.inc"
    "src/ps2_keyboard_device/keyboard_keycode_names.inc"
    "src/ps2_keyboard_device/keyboard_keycode_ascii_lowercase.inc"
    "src/ps2_keyboard_device/keyboard_scancodes_break_escaped.inc"
    "src/ps2_keyboard_device/keyboard_scancodes_make.inc"
    "src/kernel.cpp"
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
