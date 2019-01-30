#!/bin/sh
# Download and build binutils for Flapjack OS.
set -euo pipefail

mkdir -p build_toolchain
cd build_toolchain

if [ ! -f binutils-2.31.1.tar.gz ]; then
    wget http://mirrors.kernel.org/gnu/binutils/binutils-2.31.1.tar.gz
fi

if [ ! -d binutils-2.31.1 ]; then
    tar xzvf binutils-2.31.1.tar.gz
fi
