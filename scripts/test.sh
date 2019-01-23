#!/bin/sh
set -e
set -o pipefail

BUILD_PREFIX="build/"
#TEST32_BUILD_DIR="$BUILD_PREFIX/test_i386"
TEST64_BUILD_DIR="$BUILD_PREFIX/test_x86_64"

CONFIGURE="./scripts/do_configure_build.sh"
#"$CONFIGURE" "$TEST32_BUILD_DIR" -DTEST_BUILD_ARCH:string="i386"
"$CONFIGURE" "$TEST64_BUILD_DIR" -DTEST_BUILD_ARCH:string="x86_64"

BUILD="./scripts/do_build.sh"
#"$BUILD" "$TEST32_BUILD_DIR"
"$BUILD" "$TEST64_BUILD_DIR"

#pushd "$TEST32_BUILD_DIR" >/dev/null
#ctest --output-on-failure \
#    | sed -E 's/^\.\.\/\.\.\/(.*):/\1:/' \
#    | GREP_COLOR='01;36' grep --color=always -E '^.*:\d+$|$' \
#    | GREP_COLOR='01;32' grep --color=always -E '^.*\b0 tests failed.*|$' \
#    | GREP_COLOR='01;31' grep --color=always -E '^.*\b[1-9]\d* tests failed.*|$'
#popd >/dev/null
#echo

pushd "$TEST64_BUILD_DIR" >/dev/null
ctest --output-on-failure \
    | sed -E 's/^\.\.\/\.\.\/(.*):/\1:/' \
    | GREP_COLOR='01;36' grep --color=always -E '^.*:\d+:\d+:|$' \
    | GREP_COLOR='01;32' grep --color=always -E '^.*\b0 tests failed.*|$' \
    | GREP_COLOR='01;31' grep --color=always -E '^.*\b[1-9]\d* tests failed.*|$'
popd >/dev/null
