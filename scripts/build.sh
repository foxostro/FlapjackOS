#!/bin/sh
set -e

BUILD_PREFIX="build/"
X86_64_BUILD_DIR="$BUILD_PREFIX/x86_64"
I386_BUILD_DIR="$BUILD_PREFIX/i386"
TEST_BUILD_DIR="$BUILD_PREFIX/test"

if [ ! -d "$X86_64_BUILD_DIR" ]; then
    cmake -E make_directory "$X86_64_BUILD_DIR"
    cmake -E chdir "$X86_64_BUILD_DIR" cmake ../.. -DCMAKE_TOOLCHAIN_FILE="x86_64-elf.toolchain.cmake"
fi

if [ ! -d "$I386_BUILD_DIR" ]; then
    cmake -E make_directory "$I386_BUILD_DIR"
    cmake -E chdir "$I386_BUILD_DIR" cmake ../.. -DCMAKE_TOOLCHAIN_FILE="i386-elf.toolchain.cmake"
fi

if [ ! -d "$TEST_BUILD_DIR" ]; then
    cmake -E make_directory "$TEST_BUILD_DIR"
    cmake -E chdir "$TEST_BUILD_DIR" cmake ../..
fi

cmake --build "$X86_64_BUILD_DIR" || echo "ERROR: x86_64 build failed."
cmake --build "$I386_BUILD_DIR" || echo "ERROR: i386 build failed."
cmake --build "$TEST_BUILD_DIR" || echo "ERROR: unit test build failed."
