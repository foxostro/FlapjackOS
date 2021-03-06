#!/bin/sh
set -euo pipefail

PROJECT_DIR="$(pwd)"
BUILD_PREFIX="$FLAPJACKOS_BUILD_DIR/build"

#TEST32_BUILD_DIR="$BUILD_PREFIX/test_i386"
TEST64_BUILD_DIR="$BUILD_PREFIX/test_x86_64"

CONFIGURE="./scripts/do_configure_build.sh"
#"$CONFIGURE" "$PROJECT_DIR" "$TEST32_BUILD_DIR" -DTEST_BUILD_ARCH:string="i386"
"$CONFIGURE" "$PROJECT_DIR" "$TEST64_BUILD_DIR" -DTEST_BUILD_ARCH:string="x86_64"

BUILD="./scripts/do_build.sh"
#"$BUILD" "$TEST32_BUILD_DIR"
"$BUILD" "$TEST64_BUILD_DIR"

#pushd "$TEST32_BUILD_DIR" >/dev/null
#ctest --output-on-failure
#popd >/dev/null
#echo

pushd "$TEST64_BUILD_DIR" >/dev/null
ctest --output-on-failure
popd >/dev/null
