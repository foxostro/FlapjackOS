cmake_minimum_required(VERSION 3.6)
project(FlapjackOS C)

enable_language(ASM)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

# Let's stick strictly to the C11 standard.
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Setup compiler flags before building the subdirectories.
if(CMAKE_CROSSCOMPILING)

    # When cross-compiling, we want to build free-standing executables with no
    # standard library as we cannot use the libc of the host system. Also, the
    # kernel must perform some magic to boot which is never done in userspace
    # programs.
    #
    # Finally, note that we choose -O0. We do this so as to avoid any kind of
    # optimizer silliness which might break the kernel or make debugging things
    # more difficult. In the future, I should enable -O2 and fix anything that
    # breaks.
    set(CMAKE_C_FLAGS "-ffreestanding -O0 -Wall -Wextra -Werror")
    set(CMAKE_C_LINK_FLAGS "-ffreestanding -O0 -nostdlib")

else()

    # When the target system is the host system, we are building natively.
    # In this case, define a flag to indicate we are doing testing and choose
    # some rather standard compiler flags.
    set(CMAKE_C_FLAGS "-g -O0 -Wall -Wextra -Werror -DTESTING")

    # If we're building natively then we want to build tests.
    enable_testing()
    find_package(Check REQUIRED)

endif()

add_subdirectory("LibC")
set(LIBC_LIBRARIES LibC)
set(LIBC_LIBRARIES_DIR "${PROJECT_SOURCE_DIR}/LibC")
set(LIBC_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/LibC/include")

add_subdirectory("Malloc")
set(MALLOC_LIBRARIES Malloc)
set(MALLOC_LIBRARIES_DIR "${PROJECT_SOURCE_DIR}/Malloc")
set(MALLOC_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/Malloc/include")

add_subdirectory("Kernel")
set(KERNEL_LIBRARIES Kernel)
set(KERNEL_LIBRARIES_DIR "${PROJECT_SOURCE_DIR}/Kernel")
set(KERNEL_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/Kernel/include")

# If we're cross-compiling to the target architecture then build a bootable ISO.
# We cannot do this when building natively because (as far as I know) there's no
# way to use two toolchains simultaneously in CMake.
if(CMAKE_CROSSCOMPILING)

    include_directories(${KERNEL_INCLUDE_DIRS})
    link_directories(${KERNEL_LIBRARIES_DIRS})

    # The layout file specifies how the linker puts together the binary image.
    set(KERNEL_LAYOUT_FILE "${PROJECT_SOURCE_DIR}/kernel.ld")

    # Force a dependency between boot.S and the linker layout so that a build is
    # triggered when it changes.
    set_source_files_properties(
        ${PROJECT_SOURCE_DIR}/boot.S PROPERTIES OBJECT_DEPENDS ${KERNEL_LAYOUT_FILE}
    )

    # The linker produces a kernel binary image which is multiboot compliant and
    # can actually be directly booted by Qemu.
    set(KERNEL_BIN "kernel.bin")
    set(KERNEL_LINK_FLAGS "-m32 -march=i386 -T ${KERNEL_LAYOUT_FILE}")
    add_executable(${KERNEL_BIN} boot.S)
    target_link_libraries(${KERNEL_BIN}
                          ${KERNEL_LIBRARIES}
                          )
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
    set(BUILD_ISO_TOOL "${CMAKE_SOURCE_DIR}/scripts/make_iso.sh")
    add_custom_command(
        OUTPUT ${CMAKE_PROJECT_NAME}.iso
        COMMAND ${BUILD_ISO_TOOL} ${CMAKE_BINARY_DIR}/${KERNEL_BIN} "${CMAKE_SOURCE_DIR}/grub.cfg" ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.iso
        DEPENDS ${BUILD_ISO_TOOL} ${CMAKE_BINARY_DIR}/${KERNEL_BIN} "${CMAKE_SOURCE_DIR}/grub.cfg"
        COMMENT "Build the bootable ISO."
        VERBATIM
    )
    add_custom_target(
        iso ALL
        DEPENDS ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.iso
    )

endif(CMAKE_CROSSCOMPILING)
