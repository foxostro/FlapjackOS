cmake_minimum_required(VERSION 3.6)
project(FlapjackOS C)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

# Setup compiler flags.
if(CMAKE_CROSSCOMPILING)
    set(CMAKE_C_FLAGS "-std=c11 -ffreestanding -O0 -Wall -Wextra -Werror")
    set(CMAKE_C_LINK_FLAGS "-ffreestanding -O0 -nostdlib")
else()
    set(CMAKE_C_FLAGS "-std=c11 -g -O0 -Wall -Wextra -Werror -DTESTING")
endif()

# Setup build for libc static library.
set(SOURCE_FILES_LIBC
    "src/include/libc/string.h" "src/libc/string.c"
    )
add_library(StaticLibC STATIC ${SOURCE_FILES_LIBC})

# Setup other source files. We'll use these in targets below.
set(CORE_INCLUDE_FILES
    "src/include/console.h"
    "src/include/ltr.h"
    "src/include/seg.h"
    "src/include/isr_install.h"
    "src/include/halt.h"
    "src/include/idt.h"
    "src/include/readline.h"
    "src/include/kprintf.h"
    "src/include/backtrace.h"
    "src/include/misc.h"
    "src/include/timer.h"
    "src/include/pic.h"
    "src/include/keyboard.h"
    "src/include/ctypes.h"
    "src/include/panic.h"
    "src/include/malloc/malloc_zone.h"
    "src/include/malloc/malloc_block.h"
    "src/include/interrupt_asm.h"
    "src/include/inout.h"
    "src/include/tss.h"
    "src/include/gdt.h"
    "src/include/assert.h"
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

set(COMMON_SOURCE_FILES
    "src/gdt.c"
    "src/idt.c"
    "src/kprintf.c"
    "src/malloc/malloc_zone.c"
    )
source_group("Common" FILES ${COMMON_SOURCE_FILES})

# Source files for the kernel.
set(KERNEL_SOURCE_FILES
    "src/readline.c"
    "src/backtrace.c"
    "src/backtrace_asm.S"
    "src/console.c"
    "src/gdt_asm.S"
    "src/halt.S"
    "src/idt_asm.S"
    "src/inout.S"
    "src/interrupt_asm.S"
    "src/isr_install.c"
    "src/isr_wrapper_asm.S"
    "src/kernel.c"
    "src/ltr.S"
    "src/panic.c"
    "src/pic.c"
    "src/timer.c"
    "src/keyboard/keyboard.c"
    "src/boot.S"
    )
source_group("Kernel" FILES ${KERNEL_SOURCE_FILES})

# Source files for the test suite.
set(TEST_SOURCE_FILES
    "src/test/test_malloc_zone.c"
    "src/test/test_gdt.c"
    "src/test/test_idt.c"
    "src/test/test_printf.c"
    "src/test/test_main.c"
    )
source_group("Test" FILES ${TEST_SOURCE_FILES})

# Setup a build for either the kernel or the test suite.
# We can't setup both because (as far as I know) we can only have one toolchain
# for the entire CMake project.
if(CMAKE_CROSSCOMPILING)

    set(ALL_SOURCE_FILES
        ${CORE_INCLUDE_FILES}
        ${ISR_ERB_FILES}
        ${KEYBOARD_INC_FILES}
        ${COMMON_SOURCE_FILES}
        ${KERNEL_SOURCE_FILES}
        )

    enable_language(ASM)

    include_directories("src/include" "src/include/libc")

    # The layout file specifies how the linker puts together the kernel binary image.
    set(KERNEL_LAYOUT_FILE "kernel.ld")

    # Force a dependency between boot.S and the linker layout so that a build is
    # triggered when it changes.
    set_source_files_properties(
        boot.S PROPERTIES OBJECT_DEPENDS ${KERNEL_LAYOUT_FILE}
    )

    # The linker produces a kernel binary image which is multiboot compliant and
    # can actually be directly booted by Qemu.
    set(KERNEL_BIN "kernel.bin")
    set(KERNEL_LINK_FLAGS "-m32 -march=i386 -T ${KERNEL_LAYOUT_FILE}")
    add_executable(${KERNEL_BIN} ${ALL_SOURCE_FILES})
    target_link_libraries(${KERNEL_BIN} StaticLibC)
    set_target_properties(
        ${KERNEL_BIN} PROPERTIES
        LINKER_LANGUAGE C
        PREFIX ""
        SUFFIX ""
        LINK_FLAGS ${KERNEL_LINK_FLAGS}
    )

    # Add a custom target for the CD-ROM ISO image. Specify dependencies so that the
    # ISO will be rebuilt correctly after modifying the kernel binary, the Grub
    # configuration file, or the ISO build script itself.
    set(GRUB_CFG "grub.cfg")
    set(BUILD_ISO_TOOL "scripts/build_iso.sh")
    add_custom_command(
        OUTPUT ${CMAKE_PROJECT_NAME}.iso
        COMMAND ${BUILD_ISO_TOOL} ${KERNEL_BIN} ${GRUB_CFG} ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.iso
        DEPENDS ${BUILD_ISO_TOOL} ${KERNEL_BIN} ${GRUB_CFG}
        COMMENT "Build the bootable ISO."
        VERBATIM
    )
    add_custom_target(
        iso ALL
        DEPENDS ${CMAKE_PROJECT_NAME}.iso
    )

    set(ERB_TOOL "scripts/erb.sh")
    add_custom_command(
        OUTPUT src/isr_install.c
        COMMAND ${ERB_TOOL} src/isr_install.c.erb src/isr_install.c
        DEPENDS ${ERB_TOOL} src/isr_install.c.erb
        VERBATIM
    )
    add_custom_command(
        OUTPUT src/isr_wrapper_asm.S
        COMMAND ${ERB_TOOL} src/isr_wrapper_asm.S.erb src/isr_wrapper_asm.S
        DEPENDS ${ERB_TOOL} src/isr_wrapper_asm.S.erb
        VERBATIM
    )

else()

    set(ALL_SOURCE_FILES
        ${COMMON_SOURCE_FILES}
        ${TEST_SOURCE_FILES}
        )

    include_directories("src/include")

    set(TEST_RUNNER_NAME "${CMAKE_PROJECT_NAME}_Test")

    enable_testing()
    find_package(Check REQUIRED)
    include_directories(${CHECK_INCLUDE_DIRS})
    link_directories(${CHECK_LIBRARIES_DIR})
    set(LIBS ${LIBS} ${CHECK_LIBRARIES} StaticLibC)
    add_executable(${TEST_RUNNER_NAME} ${ALL_SOURCE_FILES})
    target_link_libraries(${TEST_RUNNER_NAME} ${LIBS})
    add_test(${TEST_RUNNER_NAME} ${CMAKE_CURRENT_BINARY_DIR}/${TEST_RUNNER_NAME})

endif()
