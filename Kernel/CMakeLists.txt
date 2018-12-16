cmake_minimum_required(VERSION 3.6)

set(SOURCE_FILES
    "include/backtrace.hpp" "src/backtrace.cpp"
    "include/panic.h" "src/panic.cpp"
    "include/kernel_policy.hpp" 
    "include/kernel.hpp" "src/kernel.cpp"
    "include/page_frame_allocator.hpp"
    "include/page_frame_allocator_configuration_operation.hpp"
    "include/multiboot_memory_map_page_frame_enumerator.hpp"
    "include/interrupt_dispatcher.hpp"
    "include/interrupt_lock.hpp" "src/interrupt_lock.cpp"
    "include/panic_interrupt_handler.hpp" "src/panic_interrupt_handler.cpp"
    "include/page_fault_interrupt_handler.hpp" "src/page_fault_interrupt_handler.cpp"
    )

include_directories("include"
                    ${COMMON_INCLUDE_DIRS}
                    ${MALLOC_INCLUDE_DIRS}
                    ${KERNEL_PLATFORM_SUPPORT_INCLUDE_DIRS}
                    ${BOOT_INCLUDE_DIR}
                    ${DRIVERS_INCLUDE_DIR}
                    )

link_directories(${COMMON_LIBRARIES_DIR}
                 ${MALLOC_LIBRARIES_DIRS}
                 ${KERNEL_PLATFORM_SUPPORT_LIBRARIES_DIR}
                 ${DRIVERS_LIBRARIES_DIR}
                 )

add_library(Kernel STATIC ${SOURCE_FILES})

target_link_libraries(Kernel
                      ${COMMON_LIBRARIES}
                      ${MALLOC_LIBRARIES}
                      ${KERNEL_PLATFORM_SUPPORT_LIBRARIES}
                      ${DRIVERS_LIBRARIES}
                      )

if (NOT CMAKE_CROSSCOMPILING)

    add_executable(KernelTest
                   "test/test_page_frame_allocator.cpp"
                   "test/test_page_frame_allocator_configuration_operation.cpp"
                   "test/test_main.cpp"
                   )

    target_link_libraries(KernelTest
                          Kernel
                          )

    add_test(KernelTest ${CMAKE_CURRENT_BINARY_DIR}/KernelTest)

endif (NOT CMAKE_CROSSCOMPILING)

set(KERNEL_LIBRARIES Kernel PARENT_SCOPE)
set(KERNEL_LIBRARIES_DIR "${PROJECT_SOURCE_DIR}/Kernel" PARENT_SCOPE)
set(KERNEL_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/Kernel/include" PARENT_SCOPE)
