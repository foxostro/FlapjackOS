cmake_minimum_required(VERSION 3.6)

set(SOURCE_FILES
    "include/flapjack_libc/assert.h"
    "include/flapjack_libc/string.h" "src/string.c"
    "include/flapjack_libc/stdio.h" "src/stdio.c"
    "include/flapjack_libc/stdlib.h" "src/stdlib.c"
    "include/line_editor.hpp" "src/line_editor.cpp"
    "include/console_printf.h" "src/console_printf.c"
    "include/interfaces/console_interface.h"
    "include/interfaces/keyboard_device.hpp"
    "include/ll_str.h" "src/ll_str.c" # generated
    "src/c_plus_plus_support.cpp"
    )
include_directories("include")
add_library(Common STATIC ${SOURCE_FILES})

GenerateSourceFromTemplate("${PROJECT_SOURCE_DIR}/Common/include/linked_list.h.erb"
                           "${PROJECT_SOURCE_DIR}/Common/include/ll_str.h"
                           "suffix=str type=char*")

GenerateSourceFromTemplate("${PROJECT_SOURCE_DIR}/Common/src/linked_list.c.erb"
                           "${PROJECT_SOURCE_DIR}/Common/src/ll_str.c"
                           "suffix=str type=char*")

if(CMAKE_CROSSCOMPILING)

    include_directories("include/flapjack_libc")

else(CMAKE_CROSSCOMPILING)

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

endif(CMAKE_CROSSCOMPILING)
