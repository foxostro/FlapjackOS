cmake_minimum_required(VERSION 3.6)

# We build our own libc as a static library.
# This can be linked to the kernel or to userspace programs.
set(SOURCE_FILES_LIBC
    "include/string.h"
    "src/string.c"
    )
add_library(LibC STATIC ${SOURCE_FILES_LIBC})
