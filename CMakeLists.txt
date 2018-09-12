cmake_minimum_required(VERSION 3.6)
project(FlapjackOS)

enable_language(ASM)

# Function to generate source code from a template file.
function(GenerateSourceFromTemplate
         TEMPLATE_FILENAME
         OUTPUT_FILENAME)

    set(PARAMS ${ARGV2})
    set(ERB_TOOL "${PROJECT_SOURCE_DIR}/scripts/erb.sh")
    add_custom_command(
        OUTPUT ${OUTPUT_FILENAME}
        COMMAND ${ERB_TOOL} ${TEMPLATE_FILENAME} ${OUTPUT_FILENAME} ${PARAMS}
        DEPENDS ${TEMPLATE_FILENAME} ${ERB_TOOL}
        VERBATIM
    )

endfunction(GenerateSourceFromTemplate)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(COMMON_COMPILER_FLAGS "-O2 -Wall -Wextra -Werror")

# Setup compiler flags before building the subdirectories.
if(CMAKE_CROSSCOMPILING)

    # When cross-compiling, we want to build free-standing executables with no
    # standard library as we cannot use the libc of the host system. Also, the
    # kernel must perform some magic to boot which is never done in userspace
    # programs.
    set(CMAKE_C_FLAGS "-fno-stack-protector -nostdlib -ffreestanding ${COMMON_COMPILER_FLAGS}")
    set(CMAKE_C_LINK_FLAGS "-nostdlib -ffreestanding")

    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-exceptions -fno-rtti")
    set(CMAKE_CXX_LINK_FLAGS "${CMAKE_C_LINK_FLAGS}")

else()

    # When the target system is the host system, we are building natively.
    # In this case, define a flag to indicate we are doing testing and choose
    # some rather standard compiler flags.
    set(CMAKE_C_FLAGS "-g -DTESTING ${COMMON_COMPILER_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")

    # If we're building natively then we want to build tests.
    # Tests are defined in each library.
    enable_testing()

endif()

include_directories("3rdParty")

add_subdirectory("Common")
set(COMMON_LIBRARIES Common)
set(COMMON_LIBRARIES_DIR "${PROJECT_SOURCE_DIR}/Common")
if(CMAKE_CROSSCOMPILING)
    # Some of our headers have names that collide with the host libc.
    # So, we omit those from the header search path unless building for
    # a cross-compile.
    set(COMMON_INCLUDE_DIRS
        "${PROJECT_SOURCE_DIR}/Common/include"
        "${PROJECT_SOURCE_DIR}/Common/include/flapjack_libc"
        )
else(CMAKE_CROSSCOMPILING)
    set(COMMON_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/Common/include")
endif(CMAKE_CROSSCOMPILING)

add_subdirectory("Malloc")
set(MALLOC_LIBRARIES Malloc)
set(MALLOC_LIBRARIES_DIR "${PROJECT_SOURCE_DIR}/Malloc")
set(MALLOC_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/Malloc/include")

add_subdirectory("KernelPlatformSupport")
set(KERNEL_PLATFORM_SUPPORT_LIBRARIES KernelPlatformSupport)
set(KERNEL_PLATFORM_SUPPORT_LIBRARIES_DIR "${PROJECT_SOURCE_DIR}/KernelPlatformSupport")
set(KERNEL_PLATFORM_SUPPORT_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/KernelPlatformSupport/include")

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
    set(KERNEL_LAYOUT_FILE "${PROJECT_SOURCE_DIR}/Boot/${CMAKE_SYSTEM_PROCESSOR}/kernel.ld")

    # The linker produces a kernel binary image which is multiboot compliant and
    # can actually be directly booted by Qemu.
    set(KERNEL_BIN "kernel.bin")
    set(KERNEL_LINK_FLAGS "-T ${KERNEL_LAYOUT_FILE}")
    add_executable(${KERNEL_BIN} "${PROJECT_SOURCE_DIR}/Boot/${CMAKE_SYSTEM_PROCESSOR}/head.S")
    set_target_properties(${KERNEL_BIN} PROPERTIES LINK_DEPENDS ${KERNEL_LAYOUT_FILE})
    target_link_libraries(${KERNEL_BIN}
                          ${KERNEL_LIBRARIES}
                          libgcc.a
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
