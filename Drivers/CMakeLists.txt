cmake_minimum_required(VERSION 3.6)

set(SOURCE_FILES
     "include/drivers/pc/ps2_keyboard_device.hpp" "src/drivers/pc/ps2_keyboard_device/ps2_keyboard_device.cpp"
     "src/drivers/pc/ps2_keyboard_device/keyboard_scancodes_make_escaped.inc"
     "src/drivers/pc/ps2_keyboard_device/keyboard_scancodes_break.inc"
     "src/drivers/pc/ps2_keyboard_device/keyboard_keycode_ascii_uppercase.inc"
     "src/drivers/pc/ps2_keyboard_device/keyboard_keycode_names.inc"
     "src/drivers/pc/ps2_keyboard_device/keyboard_keycode_ascii_lowercase.inc"
     "src/drivers/pc/ps2_keyboard_device/keyboard_scancodes_break_escaped.inc"
     "src/drivers/pc/ps2_keyboard_device/keyboard_scancodes_make.inc"

     "include/drivers/pc/vga_text_display_device.hpp" "src/drivers/pc/vga_text_display_device.cpp"

     "include/drivers/pc/pit_timer_device.hpp" "src/drivers/pc/pit_timer_device.cpp"
     )

include_directories("include"
                    ${COMMON_INCLUDE_DIRS}
                    ${BOOT_INCLUDE_DIR}
                    ${KERNEL_PLATFORM_SUPPORT_INCLUDE_DIRS}
                    )

link_directories(${COMMON_LIBRARIES_DIR}
                 ${KERNEL_PLATFORM_SUPPORT_LIBRARIES_DIR}
                 )

add_library(Drivers STATIC ${SOURCE_FILES})

target_link_libraries(Drivers
                      ${COMMON_LIBRARIES}
                      ${KERNEL_PLATFORM_SUPPORT_LIBRARIES}
                      )

set(DRIVERS_LIBRARIES Drivers PARENT_SCOPE)
set(DRIVERS_LIBRARIES_DIR "${PROJECT_SOURCE_DIR}/Drivers" PARENT_SCOPE)
set(DRIVERS_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Drivers/include" PARENT_SCOPE)
