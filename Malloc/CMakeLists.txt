cmake_minimum_required(VERSION 3.6)

# We build a static library for our heap memory allocator.
# This can be linked to the kernel or to userspace programs, where it will be
# used by the userspace libc.
set(SOURCE_FILES_MALLOC
    "include/malloc/malloc_zone.h"
    "include/malloc/malloc_block.h"
    "src/malloc_zone.c"
    )
include_directories("include")
add_library(Malloc STATIC ${SOURCE_FILES_MALLOC})