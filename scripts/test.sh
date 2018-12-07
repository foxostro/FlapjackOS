#!/bin/sh
set -e

BUILD_PREFIX="build/"
TEST_BUILD_DIR="$BUILD_PREFIX/test"

if [ ! -d "$TEST_BUILD_DIR" ]; then
    cmake -E make_directory "$TEST_BUILD_DIR"
    cmake -E chdir "$TEST_BUILD_DIR" cmake ../..
fi

cmake --build "$TEST_BUILD_DIR"

cd "$TEST_BUILD_DIR"
ctest --output-on-failure
