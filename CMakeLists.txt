cmake_minimum_required(VERSION 3.7)
project(FlapjackOS C)
enable_language(ASM)

set(CMAKE_C_FLAGS "-std=c11 -ffreestanding -O2 -Wall -Wextra -Werror")
set(CMAKE_C_LINK_FLAGS "-ffreestanding -O2 -nostdlib")

# Source files for the kernel. (no organization to these files yet)
set(
    SOURCE_FILES
    "kernel.c"
    "boot.S"
)

# The layout file specifies how the linker puts together the kernel binary image.
set(KERNEL_LAYOUT_FILE "kernel.ld")

# Force a dependency between boot.S and the linker layout so that a build is
# triggered when it changes.
SET_SOURCE_FILES_PROPERTIES(
    boot.S PROPERTIES OBJECT_DEPENDS ${KERNEL_LAYOUT_FILE}
)

# The linker produces a kernel binary image which is multiboot compliant and
# can actually be directly booted by Qemu.
set(KERNEL_BIN "kernel.bin")
set(KERNEL_LINK_FLAGS "-m32 -march=i386 -T ${KERNEL_LAYOUT_FILE}")
add_executable(${KERNEL_BIN} ${SOURCE_FILES})
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
