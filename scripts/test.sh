#!/bin/sh
set -e

BUILD_PREFIX="build/"
TEST32_BUILD_DIR="$BUILD_PREFIX/test_i386"
TEST64_BUILD_DIR="$BUILD_PREFIX/test_x86_64"

CONFIGURE="./scripts/do_configure_build.sh"
"$CONFIGURE" "$TEST32_BUILD_DIR" -DTEST_BUILD_ARCH:string="i386"
"$CONFIGURE" "$TEST64_BUILD_DIR" -DTEST_BUILD_ARCH:string="x86_64"

cmake --build "$TEST32_BUILD_DIR" -- -j8
cmake --build "$TEST64_BUILD_DIR" -- -j8

pushd "$TEST32_BUILD_DIR"
echo "Running tests for the i386 kernel."
ctest --output-on-failure
popd

pushd "$TEST64_BUILD_DIR"
echo "Running tests for the x86_64 kernel."
ctest --output-on-failure
popd
