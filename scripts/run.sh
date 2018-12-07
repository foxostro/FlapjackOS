#!/bin/sh
set -e

TARGET=$1
if [ -z "$TARGET" ]; then
    echo "Must specify a target, e.g., \"x86_64\"."
    exit 1
fi

BUILD_PREFIX="build"
CROSS_BUILD_DIR="$BUILD_PREFIX/$TARGET"

if [ ! -d "$CROSS_BUILD_DIR" ]; then
    cmake -E make_directory "$CROSS_BUILD_DIR"
    cmake -E chdir "$CROSS_BUILD_DIR" cmake ../.. -DCMAKE_TOOLCHAIN_FILE="$TARGET-elf.toolchain.cmake"
fi

cmake --build "$CROSS_BUILD_DIR"

erb iso="$CROSS_BUILD_DIR/FlapjackOS.iso" bochsrc.erb > bochsrc
echo > parport.out
bochs -q -rc bochs_commands.txt
