cmake_minimum_required(VERSION 3.6)

set(SOURCE_FILES
    "include/flapjack_libc/cassert"
    "include/flapjack_libc/cctype"
    "include/flapjack_libc/cstdio"
    "include/flapjack_libc/cstdlib"
    "include/flapjack_libc/cstring"
    "include/flapjack_libc/new"

    "include/flapjack_libc/assert.h"
    "include/flapjack_libc/string.h" "src/string.cpp"
    "include/flapjack_libc/stdio.h" "src/stdio.cpp"
    "include/flapjack_libc/stdlib.h" "src/stdlib.cpp"

    "include/common/line_editor.hpp" "src/line_editor.cpp"
    "include/common/console_printf.hpp" "src/console_printf.cpp"
    "include/common/linked_list.hpp"
    "include/common/console_device.hpp"
    "include/common/keyboard_device.hpp"
    "include/common/global_allocator.hpp"
    "include/common/memory_allocator.hpp"
    "include/common/minmax.hpp"
    )

if(CMAKE_CROSSCOMPILING)
list(APPEND SOURCE_FILES "src/c_plus_plus_support.cpp")
endif(CMAKE_CROSSCOMPILING)

include_directories("include")

add_library(Common STATIC ${SOURCE_FILES})

if(CMAKE_CROSSCOMPILING)

    include_directories("include/flapjack_libc")

else(CMAKE_CROSSCOMPILING)

    add_executable(CommonTest
                   "test/test_linked_list.cpp"
                   "test/test_string.cpp"
                   "test/test_stdio.cpp"
                   "test/test_main.cpp"
                   )

    target_link_libraries(CommonTest
                          Common
                          )

    add_test(CommonTest ${CMAKE_CURRENT_BINARY_DIR}/CommonTest)

endif(CMAKE_CROSSCOMPILING)
