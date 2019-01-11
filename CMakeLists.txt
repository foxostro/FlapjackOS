cmake_minimum_required(VERSION 3.6)
project(FlapjackOS)

enable_language(ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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


# Setup compiler flags before building the subdirectories.
set(COMMON_COMPILER_FLAGS "-Wall -Wextra -Werror")
if(CMAKE_CROSSCOMPILING)

    set(BUILD_ARCH "${CMAKE_SYSTEM_PROCESSOR}")
    if (BUILD_ARCH STREQUAL "i386")
        set(BUILD_ARCH_FLAGS "-m32")
    elseif (BUILD_ARCH STREQUAL "x86_64")
        set(BUILD_ARCH_FLAGS "-m64")
    endif ()

    # When cross-compiling, we want to build free-standing executables with no
    # standard library as we cannot use the libc of the host system. Also, the
    # kernel must perform some magic to boot which is never done in userspace
    # programs.
    set(COMPILER_FLAGS_C_AND_CXX "-O2 -mno-sse -mno-mmx -fno-stack-protector -nostdlib -ffreestanding")
    if ("${BUILD_ARCH}" STREQUAL "i386")
        string(APPEND COMPILER_FLAGS_C_AND_CXX " -march=i486") # necessary for GCC atomic built-ins
    elseif ("${BUILD_ARCH}" STREQUAL "x86_64")
        string(APPEND COMPILER_FLAGS_C_AND_CXX " -mno-red-zone -mcmodel=kernel")
    endif()

    set(LINKER_FLAGS_C_AND_CXX "-nostdlib -ffreestanding")

    set(CMAKE_C_FLAGS "${COMMON_COMPILER_FLAGS} ${COMPILER_FLAGS_C_AND_CXX} ${CMAKE_C_FLAGS}")
    set(CMAKE_C_LINK_FLAGS "${LINKER_FLAGS_C_AND_CXX} ${CMAKE_C_LINK_FLAGS}")

    set(CMAKE_CXX_FLAGS "-fcheck-new -fno-exceptions -fno-rtti ${COMPILER_FLAGS_C_AND_CXX} ${COMMON_COMPILER_FLAGS} ${${COMMON_COMPILER_FLAGS}}")
    set(CMAKE_CXX_LINK_FLAGS "${LINKER_FLAGS_C_AND_CXX} ${CMAKE_CXX_LINK_FLAGS}")

    set(CMAKE_ASM_FLAGS "${BUILD_ARCH_FLAGS}")

else(CMAKE_CROSSCOMPILING)

    # We define TEST_BUILD_ARCH separately from CMAKE_SYSTEM_PROCESSOR: because
    # we sometimes want to build an x86_64 test program to test our i386 code,
    # and we need to carry that information through to various submodules.
    SET(TEST_BUILD_ARCH "x86_64" CACHE STRING "The architecture of the system-under-test. Though, all tests are built in the architecture of the host machine.")

    set(BUILD_ARCH "${TEST_BUILD_ARCH}")
    if (BUILD_ARCH STREQUAL "i386")
        set(BUILD_ARCH_FLAGS "-m32")
    elseif (BUILD_ARCH STREQUAL "x86_64")
        set(BUILD_ARCH_FLAGS "-m64")
    endif ()

    # When the target system is the host system, we are building natively.
    # In this case, define a flag to indicate we are doing testing and choose
    # some rather standard compiler flags.
    # Disable optimization so unit tests build faster. (25s versus 55s)
    set(COMPILER_FLAGS_C_AND_CXX "${BUILD_ARCH_FLAGS} -O0 -g -DTESTING")
    set(CMAKE_C_FLAGS "${COMPILER_FLAGS_C_AND_CXX} ${COMMON_COMPILER_FLAGS} ${CMAKE_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${COMPILER_FLAGS_C_AND_CXX} ${COMMON_COMPILER_FLAGS} ${CMAKE_CXX_FLAGS}")
    set(CMAKE_ASM_FLAGS "${BUILD_ARCH_FLAGS}")

    # If we're building natively then we want to build tests.
    # Tests are defined in each library.
    enable_testing()

endif(CMAKE_CROSSCOMPILING)


include_directories("3rdParty")

add_subdirectory("Common")
add_subdirectory("Boot")
add_subdirectory("Kernel")
add_subdirectory("User")


# If we're cross-compiling to the target architecture then build a bootable ISO.
# We cannot do this when building natively because (as far as I know) there's no
# way to use two toolchains simultaneously in CMake.
if(CMAKE_CROSSCOMPILING)

    # Add a target for the kernel binary image.
    # We'll link the static libraries with the boot code using a custom linker
    # script. The kernel has special linking requirements which vary between
    # architectures. So, we generate the linker script from a template file too.
    # Note that we specify the boot source file explicitly instead of, say,
    # building a boot static library. The reason for this is that CMake insists
    # that it needs at least one source file for the call to add_executable()
    set(KERNEL_BIN "kernel.bin")
    set(CPP_TOOL "${PROJECT_SOURCE_DIR}/scripts/preprocessor.sh")
    set(BOOT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/Boot/${CMAKE_SYSTEM_PROCESSOR}")
    set(KERNEL_LAYOUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/kernel.ld")
    set(KERNEL_LAYOUT_FILE_TEMPLATE "${PROJECT_SOURCE_DIR}/Boot/common/kernel.ld")
    include_directories(${KERNEL_INCLUDE_DIRS} ${BOOT_INCLUDE_DIR})
    link_directories(${KERNEL_LIBRARIES_DIRS})
    add_executable(${KERNEL_BIN} "${BOOT_SOURCE_DIR}/boot.S")
    if (BUILD_ARCH STREQUAL "x86_64")
        set(PREPROCESSOR_ARCHITECTURE "-m64")
    else()
        set(PREPROCESSOR_ARCHITECTURE "-m32")
    endif()
    add_custom_command(TARGET ${KERNEL_BIN}
        PRE_LINK
        COMMAND "${CPP_TOOL}" "${KERNEL_LAYOUT_FILE_TEMPLATE}" "${KERNEL_LAYOUT_FILE}" "-I ${BOOT_INCLUDE_DIR} ${PREPROCESSOR_ARCHITECTURE}"
        DEPENDS "${CPP_TOOL}" "${KERNEL_LAYOUT_FILE_TEMPLATE}" "${BOOT_INCLUDE_DIR}/boot.h"
    )
    target_link_libraries(${KERNEL_BIN}
        ${KERNEL_LIBRARIES}
        libgcc.a
    )
    set_target_properties(${KERNEL_BIN} PROPERTIES
        LINKER_LANGUAGE C
        PREFIX ""
        SUFFIX ""
        LINK_FLAGS "-T ${KERNEL_LAYOUT_FILE}"
        LINK_DEPENDS "${KERNEL_LAYOUT_FILE_TEMPLATE}"
    )
    
    # Add a custom target for the CD-ROM ISO image. Specify dependencies so that the
    # ISO will be rebuilt correctly after modifying the kernel binary, the Grub
    # configuration file, or the ISO build script itself.
    set(BUILD_ISO_TOOL "${CMAKE_SOURCE_DIR}/scripts/make_iso.sh")
    add_custom_command(
        OUTPUT ${CMAKE_PROJECT_NAME}.iso
        COMMAND ${BUILD_ISO_TOOL} ${CMAKE_BINARY_DIR}/${KERNEL_BIN} "${CMAKE_SOURCE_DIR}/grub.cfg.erb" ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.iso ${MODULES}
        DEPENDS ${BUILD_ISO_TOOL} ${CMAKE_BINARY_DIR}/${KERNEL_BIN} "${CMAKE_SOURCE_DIR}/grub.cfg.erb" ${MODULES}
        COMMENT "Build the bootable ISO."
        VERBATIM
    )
    add_custom_target(
        iso ALL
        DEPENDS ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.iso
    )

endif(CMAKE_CROSSCOMPILING)
