#!/bin/sh
set -e

TARGET=$1
if [ -z "$TARGET" ]; then
    echo "Must specify a target, e.g., \"x86_64\"."
    exit 1
fi

BUILD_PREFIX="build"
CROSS_BUILD_DIR="$BUILD_PREFIX/$TARGET"
CONFIGURE="./scripts/do_configure_build.sh"
"$CONFIGURE" "$CROSS_BUILD_DIR" -DCMAKE_TOOLCHAIN_FILE="$TARGET-elf.toolchain.cmake"

cmake --build "$CROSS_BUILD_DIR" -- -j8

erb iso="$CROSS_BUILD_DIR/FlapjackOS.iso" bochsrc.erb > bochsrc
echo > parport.out
bochs -q -rc bochs_commands.txt
