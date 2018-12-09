#!/bin/sh
set -e

BUILD_PREFIX="build/"
X86_64_BUILD_DIR="$BUILD_PREFIX/x86_64"
I386_BUILD_DIR="$BUILD_PREFIX/i386"
TEST32_BUILD_DIR="$BUILD_PREFIX/test_i386"
TEST64_BUILD_DIR="$BUILD_PREFIX/test_x86_64"

if [ ! -d "$X86_64_BUILD_DIR" ]; then
    cmake -E make_directory "$X86_64_BUILD_DIR"
    cmake -E chdir "$X86_64_BUILD_DIR" cmake ../.. -DCMAKE_TOOLCHAIN_FILE="x86_64-elf.toolchain.cmake"
fi

if [ ! -d "$I386_BUILD_DIR" ]; then
    cmake -E make_directory "$I386_BUILD_DIR"
    cmake -E chdir "$I386_BUILD_DIR" cmake ../.. -DCMAKE_TOOLCHAIN_FILE="i386-elf.toolchain.cmake"
fi

if [ ! -d "$TEST32_BUILD_DIR" ]; then
    cmake -E make_directory "$TEST32_BUILD_DIR"
    cmake -E chdir "$TEST32_BUILD_DIR" cmake ../.. -DBUILD_32_BIT_TESTS=ON
fi

if [ ! -d "$TEST64_BUILD_DIR" ]; then
    cmake -E make_directory "$TEST64_BUILD_DIR"
    cmake -E chdir "$TEST64_BUILD_DIR" cmake ../.. -DBUILD_32_BIT_TESTS=OFF
fi

cmake --build "$X86_64_BUILD_DIR" || echo "ERROR: x86_64 build failed."
cmake --build "$I386_BUILD_DIR" || echo "ERROR: i386 build failed."
cmake --build "$TEST32_BUILD_DIR" || echo "ERROR: i386 unit test build failed."
cmake --build "$TEST64_BUILD_DIR" || echo "ERROR: x86_64 unit test build failed."
