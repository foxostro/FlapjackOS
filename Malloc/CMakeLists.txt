cmake_minimum_required(VERSION 3.6)

# We build a static library for our heap memory allocator.
# This can be linked to the kernel or to userspace programs, where it will be
# used by the userspace libc.
set(SOURCE_FILES_MALLOC
    "include/malloc/malloc_zone.hpp"
    "src/malloc_zone.cpp"
    )

include_directories(BEFORE "include")
add_library(Malloc STATIC ${SOURCE_FILES_MALLOC})

# When cross-compiling, build using libCommon, which implements a subset of
# the C standard library. When we build unit tests, we use the system libc.
include_directories(BEFORE ${COMMON_INCLUDE_DIRS} )
target_link_libraries(Malloc 
                      ${COMMON_LIBRARIES}
                      )

if(NOT CMAKE_CROSSCOMPILING)

    add_executable(MallocTest
                   "test/test_malloc_zone.cpp"
                   "test/test_main.cpp"
                   )

    target_link_libraries(MallocTest
                          Malloc
                          )

    add_test(MallocTest ${CMAKE_CURRENT_BINARY_DIR}/MallocTest)

endif(NOT CMAKE_CROSSCOMPILING)

set(MALLOC_LIBRARIES Malloc PARENT_SCOPE)
set(MALLOC_LIBRARIES_DIR "${PROJECT_SOURCE_DIR}/Malloc" PARENT_SCOPE)
set(MALLOC_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/Malloc/include" PARENT_SCOPE)
