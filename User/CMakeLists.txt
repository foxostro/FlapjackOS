cmake_minimum_required(VERSION 3.6)

if (CMAKE_CROSSCOMPILING)
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -ffreestanding -nostdlib")
add_executable(Test1 "src/test1.S")
add_executable(Test2 "src/test2.S")
set(MODULES
    "${CMAKE_CURRENT_BINARY_DIR}/Test1"
    "${CMAKE_CURRENT_BINARY_DIR}/Test2"
    PARENT_SCOPE
    )
endif (CMAKE_CROSSCOMPILING)
