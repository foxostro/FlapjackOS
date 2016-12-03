cmake_minimum_required(VERSION 3.7)
project(FlapjackOS)

# In lieu of a toolchain file, setup the cross compiler right here.
set(CMAKE_ASM_COMPILER i386-elf-gcc)
enable_language(ASM) # Order matters: Do this after setting CMAKE_ASM_COMPILER.
set(CMAKE_C_COMPILER i386-elf-gcc)
set(CMAKE_C_FLAGS "-std=gnu11 -ffreestanding -O2 -Wall -Wextra -Werror")

# If we don't force CMAKE_C_LINK_FLAGS to an empty string then CMake tacks on a
# bunch of additional link options that cause kernel.bin to build incorrectly
# and crash.
set(CMAKE_C_LINK_FLAGS "")

# Source files for the kernel. (no organization to these files yet)
set(SOURCE_FILES
    "kernel.c"
    "boot.S"
    )

# The linker produces a kernel binary image which is multiboot compliant and
# can actually be directly booted by Qemu.
set(KERNEL_BIN "kernel.bin")
set(KERNEL_LINK_FLAGS "-T linker.ld -ffreestanding -O2 -nostdlib -lgcc") # TODO: Specify for Cmake that kernel.bin depends on linker.ld
add_executable(${KERNEL_BIN} ${SOURCE_FILES})
set_target_properties(${KERNEL_BIN} PROPERTIES
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
add_custom_command(OUTPUT ${CMAKE_PROJECT_NAME}.iso
                   COMMAND ${BUILD_ISO_TOOL} ${KERNEL_BIN} ${GRUB_CFG} ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.iso
                   DEPENDS ${BUILD_ISO_TOOL} ${KERNEL_BIN} ${GRUB_CFG}
                   COMMENT "Build the bootable ISO."
                   VERBATIM
                   )
add_custom_target(iso ALL
                  DEPENDS ${CMAKE_PROJECT_NAME}.iso
                  )
