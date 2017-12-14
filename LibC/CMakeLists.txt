cmake_minimum_required(VERSION 3.6)

# We build our own libc as a static library.
# This can be linked to the kernel or to userspace programs.
set(SOURCE_FILES_LIBC
    "include/string.h" "src/string.c"
    "include/stdio.h" "src/stdio.c"
    )
add_library(LibC STATIC ${SOURCE_FILES_LIBC})

if(NOT CMAKE_CROSSCOMPILING)

    include_directories(${CHECK_INCLUDE_DIRS})

    link_directories(${CHECK_LIBRARIES_DIR})

    add_executable(LibCTest
                   "test/test_string.c"
                   "test/test_stdio.c"
                   "test/test_main.c"
                   )

    target_link_libraries(LibCTest
                          LibC
                          ${CHECK_LIBRARIES}
                          )

    add_test(LibCTest ${CMAKE_CURRENT_BINARY_DIR}/LibCTest)

endif()