#!/bin/sh
set -e

BUILD_PREFIX="build/"
TEST32_BUILD_DIR="$BUILD_PREFIX/test_i386"
TEST64_BUILD_DIR="$BUILD_PREFIX/test_x86_64"

if [ ! -d "$TEST32_BUILD_DIR" ]; then
    cmake -E make_directory "$TEST32_BUILD_DIR"
    cmake -E chdir "$TEST32_BUILD_DIR" cmake ../.. -DTEST_BUILD_ARCH:string="i386"
fi

if [ ! -d "$TEST64_BUILD_DIR" ]; then
    cmake -E make_directory "$TEST64_BUILD_DIR"
    cmake -E chdir "$TEST64_BUILD_DIR" cmake ../.. -DTEST_BUILD_ARCH:string="x86_64"
fi

cmake --build "$TEST32_BUILD_DIR"
cmake --build "$TEST64_BUILD_DIR"

pushd "$TEST32_BUILD_DIR"
echo "Running tests for the i386 kernel."
ctest --output-on-failure
popd

pushd "$TEST64_BUILD_DIR"
echo "Running tests for the x86_64 kernel."
ctest --output-on-failure
popd
