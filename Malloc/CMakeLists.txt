cmake_minimum_required(VERSION 3.6)

# We build a static library for our heap memory allocator.
# This can be linked to the kernel or to userspace programs, where it will be
# used by the userspace libc.
set(SOURCE_FILES_MALLOC
    "include/malloc/malloc_zone.h"
    "src/malloc_zone.c"
    )

include_directories(BEFORE "include")
add_library(Malloc STATIC ${SOURCE_FILES_MALLOC})

if(CMAKE_CROSSCOMPILING)

  # When cross-compiling, build using libCommon, which implements a subset of
  # the C standard library. When we build unit tests, we use the system libc.
  include_directories(BEFORE ${COMMON_INCLUDE_DIRS} )
  target_link_libraries(Malloc 
                        ${COMMON_LIBRARIES}
                        )

else(CMAKE_CROSSCOMPILING)

    include_directories(${CHECK_INCLUDE_DIRS})
    link_directories(${CHECK_LIBRARIES_DIR})

    add_executable(MallocTest
                   "test/test_malloc_zone.c"
                   "test/test_main.c"
                   )

    target_link_libraries(MallocTest
                          Malloc
                          ${CHECK_LIBRARIES}
                          )

    add_test(MallocTest ${CMAKE_CURRENT_BINARY_DIR}/MallocTest)

endif(CMAKE_CROSSCOMPILING)