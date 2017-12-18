cmake_minimum_required(VERSION 3.6)

set(SOURCE_FILES
    "include/flapjack_libc/assert.h"
    "include/flapjack_libc/string.h" "src/string.c"
    "include/flapjack_libc/stdio.h" "src/stdio.c"
    "include/line_editor.h" "include/line_editor_private.h" "src/line_editor.c"
    "include/console_printf.h" "src/console_printf.c"
    "include/interfaces/console_interface.h"
    "include/interfaces/keyboard_interface.h"
    )
include_directories("include")
add_library(Common STATIC ${SOURCE_FILES})

if(NOT CMAKE_CROSSCOMPILING)

    include_directories(${CHECK_INCLUDE_DIRS})
    link_directories(${CHECK_LIBRARIES_DIR})

    add_executable(CommonTest
                   "test/test_string.c"
                   "test/test_stdio.c"
                   "test/test_main.c"
                   )

    target_link_libraries(CommonTest
                          Common
                          ${CHECK_LIBRARIES}
                          )

    add_test(CommonTest ${CMAKE_CURRENT_BINARY_DIR}/CommonTest)

endif()