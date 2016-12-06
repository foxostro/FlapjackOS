cmake_minimum_required(VERSION 3.6)
project(FlapjackOS C)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

set(COMMON_SOURCE_FILES
    "src/kprintf.c"
    )

include_directories("src/include")

if(CMAKE_CROSSCOMPILING)

    enable_language(ASM)

    # Source files for the kernel. (no organization to these files yet)
    set(KERNEL_SOURCE_FILES
        ${COMMON_SOURCE_FILES}
        "src/kernel.c"
        "src/boot.S"
        )

    set(CMAKE_C_FLAGS "-std=c11 -ffreestanding -O2 -Wall -Wextra -Werror")
    set(CMAKE_C_LINK_FLAGS "-ffreestanding -O2 -nostdlib")

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
    add_executable(${KERNEL_BIN} ${KERNEL_SOURCE_FILES})
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

else()

    set(TEST_RUNNER_NAME "${CMAKE_PROJECT_NAME}_Test")

    # Source files for the test suite.
    set(TEST_SOURCE_FILES
         ${COMMON_SOURCE_FILES}
        "src/test/test_printf.c"
        "src/test/test_main.c"
        )

    enable_testing()
    find_package(Check REQUIRED)
    include_directories(${CHECK_INCLUDE_DIRS})
    link_directories(${CHECK_LIBRARIES_DIR})
    set(LIBS ${LIBS} ${CHECK_LIBRARIES})
    set(CMAKE_C_FLAGS "-std=c11 -g -O2 -Wall -Wextra -Werror")
    add_executable(${TEST_RUNNER_NAME} ${TEST_SOURCE_FILES})
    target_link_libraries(${TEST_RUNNER_NAME} ${LIBS})
    add_test(${TEST_RUNNER_NAME} ${CMAKE_CURRENT_BINARY_DIR}/${TEST_RUNNER_NAME})

endif()
