cmake_minimum_required(VERSION 3.6)

set(SOURCE_FILES
    "include/flapjack_libc/cctype"
    "include/flapjack_libc/ctype.h"
    "include/flapjack_libc/ctype.inc"

    "include/flapjack_libc/cassert"
    "include/flapjack_libc/assert.h"
    
    "include/flapjack_libc/cstdio"
    "include/flapjack_libc/cstdlib"
    "include/flapjack_libc/cstring"

    "include/flapjack_libc/string.h" "src/string.cpp"
    "include/flapjack_libc/stdio.h" "src/stdio.cpp"
    "include/flapjack_libc/stdlib.h" "src/stdlib.cpp"

    "include/common/timer_device.hpp"
    "include/common/keyboard_device.hpp"
    "include/common/global_allocator.hpp"
    "include/common/memory_allocator.hpp"
    "include/common/terminal_metrics.hpp"
    "include/common/text_display_device.hpp"
    
    "include/common/misc.h"
    "include/common/vector.hpp"
    "include/common/linked_list.hpp"
    "include/common/ring_buffer.hpp"
    "include/common/vec2.hpp"
    "include/common/minmax.hpp"
    "include/common/static_stack.hpp"

    "include/common/line_editor.hpp" "src/line_editor.cpp"
    "include/common/text_line.hpp" "src/text_line.cpp"
    "include/common/text_output_stream.hpp"
    "include/common/text_terminal.hpp" "src/text_terminal.cpp"
    "include/common/bit_array.hpp"
    "include/common/spin_lock.hpp" "include/common/spin_lock.h" "src/spin_lock.c"
    "include/common/mutex.hpp" "src/mutex.cpp"
    "include/common/interrupt_lock.hpp" "src/interrupt_lock.cpp"
    "include/common/lock_guard.hpp"
    "include/common/shared_pointer.hpp"
    "include/common/unique_pointer.hpp"
    "include/common/cmpxchg.h" "src/cmpxchg.c"
    "include/common/elf.hpp"
    "include/common/elf32.hpp"
    "include/common/elf64.hpp"
    "include/common/elf32_parser.hpp"
    "include/common/elf64_parser.hpp"
    "include/common/data.hpp"
    "include/common/error.hpp"
    "include/common/either.hpp"
    "include/common/optional.hpp"
    "include/common/expected.hpp"
    "include/common/logger.hpp" "src/logger.cpp"
    "include/common/string_to_long_converter.hpp"

    "include/malloc/malloc_zone.hpp" "src/malloc_zone.cpp"
    )

if(CMAKE_CROSSCOMPILING)
    list(APPEND SOURCE_FILES
         "src/c_plus_plus_support.cpp"
         )
    list(APPEND SOURCE_FILES
         # OS Support Files and Stubs
         "src/libcxxrt/sched.h" "src/libcxxrt/sched.cpp"
         "src/libcxxrt/pthread.h" "src/libcxxrt/pthread.cpp"
         "src/libcxxrt/dlfcn.h" "src/libcxxrt/dlfcn.cpp"

         # libcxxrt proper
         "src/libcxxrt/dynamic_cast.cc"
         "src/libcxxrt/exception.cc"
         "src/libcxxrt/guard.cc"
         "src/libcxxrt/stdexcept.cc"
         "src/libcxxrt/typeinfo.cc"
         "src/libcxxrt/memory.cc"
         "src/libcxxrt/auxhelper.cc"
         "src/libcxxrt/libelftc_dem_gnu3.c"
         )
    add_definitions(-D_GNU_SOURCE)
endif(CMAKE_CROSSCOMPILING)

include_directories("include")

add_library(Common STATIC ${SOURCE_FILES})

if(CMAKE_CROSSCOMPILING)

    include_directories("include/flapjack_libc")

else(CMAKE_CROSSCOMPILING)

    add_executable(CommonTest
                   "test/dummy_text_display_device.hpp"
                   "test/dummy_keyboard_device.hpp"
                   "test/test_bit_array.cpp"
                   "test/test_linked_list.cpp"
                   "test/test_string.cpp"
                   "test/test_main.cpp"
                   "test/test_text_terminal.cpp"
                   "test/test_text_line.cpp"
                   "test/test_vector.cpp"
                   "test/test_line_editor.cpp"
                   "test/test_ring_buffer.cpp"
                   "test/test_stdio.cpp"
                   "test/test_shared_pointer.cpp"
                   "test/test_elf32.cpp"
                   "test/test_elf64.cpp"
                   "test/test_malloc_zone.cpp"
                   "test/test_either.cpp"
                   "test/test_optional.cpp"
                   "test/test_expected.cpp"
                   )

    target_link_libraries(CommonTest
                          Common
                          )

    add_test(CommonTest ${CMAKE_CURRENT_BINARY_DIR}/CommonTest)

endif(CMAKE_CROSSCOMPILING)

set(COMMON_LIBRARIES Common PARENT_SCOPE)
set(COMMON_LIBRARIES_DIR "${PROJECT_SOURCE_DIR}/Common" PARENT_SCOPE)
if(CMAKE_CROSSCOMPILING)
    # Some of our headers have names that collide with the host libc.
    # So, we omit those from the header search path unless building for
    # a cross-compile.
    set(COMMON_INCLUDE_DIRS
        "${PROJECT_SOURCE_DIR}/Common/include"
        "${PROJECT_SOURCE_DIR}/Common/include/flapjack_libc"
        PARENT_SCOPE)
else(CMAKE_CROSSCOMPILING)
    set(COMMON_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/Common/include" PARENT_SCOPE)
endif(CMAKE_CROSSCOMPILING)
