#!/bin/sh
set -e

CROSS_BUILD_DIR="build_cross"
HOST_BUILD_DIR="build_host"

if [ ! -d "$CROSS_BUILD_DIR" ]; then
    cmake -E make_directory "$CROSS_BUILD_DIR"
    cmake -E chdir "$CROSS_BUILD_DIR" cmake .. -DCMAKE_TOOLCHAIN_FILE=i386-elf.toolchain.cmake
fi

if [ ! -d "$HOST_BUILD_DIR" ]; then
    cmake -E make_directory "$HOST_BUILD_DIR"
    cmake -E chdir "$HOST_BUILD_DIR" cmake ..
fi

cmake --build "$CROSS_BUILD_DIR"
cmake --build "$HOST_BUILD_DIR"
