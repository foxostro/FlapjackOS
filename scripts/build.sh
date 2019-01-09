#!/bin/sh
set -e

BUILD_PREFIX="build/"
X86_64_BUILD_DIR="$BUILD_PREFIX/x86_64"
I386_BUILD_DIR="$BUILD_PREFIX/i386"
TEST32_BUILD_DIR="$BUILD_PREFIX/test_i386"
TEST64_BUILD_DIR="$BUILD_PREFIX/test_x86_64"

CONFIGURE="./scripts/do_configure_build.sh"
"$CONFIGURE" "$X86_64_BUILD_DIR" -DCMAKE_TOOLCHAIN_FILE="x86_64-elf.toolchain.cmake"
"$CONFIGURE" "$I386_BUILD_DIR" -DCMAKE_TOOLCHAIN_FILE="i386-elf.toolchain.cmake"
"$CONFIGURE" "$TEST32_BUILD_DIR" -DTEST_BUILD_ARCH:string="i386"
"$CONFIGURE" "$TEST64_BUILD_DIR" -DTEST_BUILD_ARCH:string="x86_64"

cmake --build "$X86_64_BUILD_DIR" -- -j8 || echo "ERROR: x86_64 build failed."
cmake --build "$I386_BUILD_DIR" -- -j8 || echo "ERROR: i386 build failed."
cmake --build "$TEST32_BUILD_DIR" -- -j8 || echo "ERROR: i386 unit test build failed."
cmake --build "$TEST64_BUILD_DIR" -- -j8 || echo "ERROR: x86_64 unit test build failed."
