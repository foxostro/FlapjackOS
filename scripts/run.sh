#!/bin/sh
set -e

CROSS_BUILD_DIR="build_cross"

if [ ! -d "$CROSS_BUILD_DIR" ]; then
    cmake -E make_directory "$CROSS_BUILD_DIR"
    cmake -E chdir "$CROSS_BUILD_DIR" cmake .. -DCMAKE_TOOLCHAIN_FILE=i386-elf.toolchain.cmake
fi

cmake --build "$CROSS_BUILD_DIR"

echo > parport.out && bochs -q -rc bochs_commands.txt
