#!/bin/sh
# Download and build binutils for Flapjack OS.
set -euo pipefail

PREFIX="$HOME/opt/cross"
TARGET=$1
PARALLEL_MAKE=-j4

mkdir -p build_toolchain
cd build_toolchain

# Build binutils
mkdir -p build-binutils
pushd build-binutils
../binutils-2.31.1/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --disable-werror
make $PARALLEL_MAKE
make install
popd
