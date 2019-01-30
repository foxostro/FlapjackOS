#!/bin/sh
# Download and build a GCC cross compiler for Flapjack OS.
# See also <https://wiki.osdev.org/GCC_Cross-Compiler>
set -euxo pipefail

PREFIX="$HOME/opt/cross"
TARGET=$1
PATH="$PREFIX/bin:$PATH"
PARALLEL_MAKE=-j4

mkdir -p build_toolchain
cd build_toolchain

# Begin building GCC
rm -rf build-gcc
mkdir build-gcc
pushd build-gcc
../gcc-8.2.0/configure --target=$TARGET --prefix="$PREFIX" --enable-languages=c,c++ --with-newlib --with-gmp=/opt/local --with-iconv=/opt/local --disable-nls --disable-werror
make $PARALLEL_MAKE all-gcc
make install-gcc
popd

# Build the C standard library
# Our freestanding kernel won't actually use this, but we need to build it in
# order to get libstdc++ to build. We won't actually use that, but we need to
# build it in order to get the headers that a compliant C++ compiler is required
# to provide in a freestanding environment.
rm -rf build-newlib
mkdir build-newlib
pushd build-newlib
../newlib-master/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --disable-werror
make $PARALLEL_MAKE
make install
popd

# Circle back and build the rest of GCC, including the C++ standard library
pushd build-gcc
make $PARALLEL_MAKE all
make install
popd
