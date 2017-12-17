cmake_minimum_required(VERSION 3.6)

set(CORE_INCLUDE_FILES
    "include/console.h"
    "include/ltr.h"
    "include/seg.h"
    "include/isr_install.h"
    "include/halt.h"
    "include/idt.h"
    "include/backtrace.h"
    "include/misc.h"
    "include/timer.h"
    "include/pic.h"
    "include/keyboard.h"
    "include/ctypes.h"
    "include/panic.h"
    "include/interrupt_asm.h"
    "include/inout.h"
    "include/tss.h"
    "include/gdt.h"
    )
source_group("Headers" FILES ${CORE_INCLUDE_FILES})

set(ISR_ERB_FILES
    "src/isr_install.c.erb"
    "src/isr_wrapper_asm.S.erb"
    )
source_group("ISR Template Files" FILES ${ISR_ERB_FILES})

set(KEYBOARD_INC_FILES
    "src/keyboard/keyboard_scancodes_make_escaped.inc"
    "src/keyboard/keyboard_scancodes_break.inc"
    "src/keyboard/keyboard_keycode_ascii_uppercase.inc"
    "src/keyboard/keyboard_keycode_names.inc"
    "src/keyboard/keyboard_keycode_ascii_lowercase.inc"
    "src/keyboard/keyboard_scancodes_break_escaped.inc"
    "src/keyboard/keyboard_scancodes_make.inc"
    )
source_group("Keyboard Inc" FILES ${KEYBOARD_INC_FILES})

set(SOURCE_FILES
    "src/gdt.c"
    "src/idt.c"
    )

# Platform specific source files for the kernel.
# These cannot be used in unit tests on the host because they cannot be built
# for the host architecture.
set(PLATFORM_SPECIFIC_SOURCE_FILES
    "src/backtrace.c"
    "src/backtrace_asm.S"
    "src/console.c"
    "src/gdt_asm.S"
    "src/halt.S"
    "src/idt_asm.S"
    "src/inout.S"
    "src/interrupt_asm.S"
    "${CMAKE_CURRENT_BINARY_DIR}/isr_install.c"
    "${CMAKE_CURRENT_BINARY_DIR}/isr_wrapper_asm.S"
    "src/kernel.c"
    "src/ltr.S"
    "src/panic.c"
    "src/pic.c"
    "src/timer.c"
    "src/keyboard/keyboard.c"
    )
source_group("Platform-specific" FILES ${PLATFORM_SPECIFIC_SOURCE_FILES})

set(ALL_PLATFORM_SPECIFIC_SOURCE_FILES
    ${CORE_INCLUDE_FILES}
    ${ISR_ERB_FILES}
    ${KEYBOARD_INC_FILES}
    ${SOURCE_FILES}
    ${PLATFORM_SPECIFIC_SOURCE_FILES}
    )


include_directories("include"
                    ${COMMON_INCLUDE_DIRS}
                    ${MALLOC_INCLUDE_DIRS}
                    )

link_directories(${COMMON_LIBRARIES_DIR}
                 ${MALLOC_LIBRARIES_DIRS}
                 )

# Generate source files from template files.
function(GenerateSourceFromTemplate TEMPLATE_FILENAME)
    set(ERB_TOOL "${PROJECT_SOURCE_DIR}/scripts/erb.sh")
    string(REGEX REPLACE "\\.[^.]*$" "" OUTPUT_FILENAME ${TEMPLATE_FILENAME})
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_FILENAME}
        COMMAND ${ERB_TOOL} ${PROJECT_SOURCE_DIR}/Kernel/src/${TEMPLATE_FILENAME} ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_FILENAME}
        DEPENDS ${PROJECT_SOURCE_DIR}/Kernel/src/${TEMPLATE_FILENAME}
        VERBATIM
    )
endfunction(GenerateSourceFromTemplate)

GenerateSourceFromTemplate(isr_install.c.erb)
GenerateSourceFromTemplate(isr_wrapper_asm.S.erb)

# We can only build the kernel static library when cross-compiling to the target
# architecture. Also, it's not very testable since it will try to actually make
# low-level calls not permitted in userspace. So, we build a small test program
# which uses only a small subset of the source files here.
#
# TODO: I'd like to break each testable component off into it's own static
# library and give it its own unittest suite. See how I've done this with
# libCommon and Malloc for details.
if(CMAKE_CROSSCOMPILING)

    add_library(Kernel STATIC ${ALL_PLATFORM_SPECIFIC_SOURCE_FILES})

    target_link_libraries(Kernel
                          ${COMMON_LIBRARIES}
                          ${MALLOC_LIBRARIES}
                          )

else(CMAKE_CROSSCOMPILING)

    include_directories(${CHECK_INCLUDE_DIRS})
    link_directories(${CHECK_LIBRARIES_DIR})

    add_executable(KernelTest
                   ${SOURCE_FILES}
                   "test/test_gdt.c"
                   "test/test_idt.c"
                   "test/test_main.c"
                   )

    target_link_libraries(KernelTest
                          ${COMMON_LIBRARIES}
                          ${MALLOC_LIBRARIES}
                          ${CHECK_LIBRARIES}
                          )

    add_test(KernelTest ${CMAKE_CURRENT_BINARY_DIR}/KernelTest)

endif(CMAKE_CROSSCOMPILING)