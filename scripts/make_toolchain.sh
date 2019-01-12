#!/bin/sh
# Download and build a toolchain for building Flapjack OS.
# See also <https://wiki.osdev.org/GCC_Cross-Compiler>
set -e

# If this ends in a trailing semicolon then the GCC configure script will fail
# with an error message about how the library path should not contain the
# current directory. Workaround the problem by unsetting the var entirely.
unset LIBRARY_PATH

PREFIX="$HOME/opt/cross"
TARGET=$1
PATH="$PREFIX/bin:$PATH"
PARALLEL_MAKE=-j4
CONFIGURATION_OPTIONS="--disable-nls --disable-werror"

echo "Requesting root in order to install dependencies from MacPorts."
sudo port install gmp mpfr libmpc libiconv

mkdir -p build_toolchain
cd build_toolchain

# Download packages
if [ ! -f gcc-8.2.0.tar.gz ]; then
    wget http://www.netgull.com/gcc/releases/gcc-8.2.0/gcc-8.2.0.tar.gz
fi

if [ ! -f binutils-2.31.1.tar.gz ]; then
    wget http://mirrors.kernel.org/gnu/binutils/binutils-2.31.1.tar.gz
fi

if [ ! -f newlib-master.zip ]; then
    wget -nc -O newlib-master.zip https://github.com/bminor/newlib/archive/master.zip
fi

# Extract packages
if [ ! -d binutils-2.31.1 ]; then
    tar xzvf binutils-2.31.1.tar.gz
fi

if [ ! -d gcc-8.2.0 ]; then
    tar xzvf gcc-8.2.0.tar.gz
fi

if [ ! -d newlib-master ]; then
    unzip -qo newlib-master.zip
fi

# Build binutils
mkdir -p build-binutils
pushd build-binutils
../binutils-2.31.1/configure --target=$TARGET --prefix="$PREFIX" $CONFIGURATION_OPTIONS
make $PARALLEL_MAKE
make install
popd

# Patch GCC
if [[ $TARGET == "x86_64-elf" ]]; then
mkdir -p ./gcc-8.2.0/gcc/config/i386/
mv ../scripts/patch-gcc/t-x86_64-elf ./gcc-8.2.0/gcc/config/i386/
patch -p0 ./gcc-8.2.0/gcc/config.gcc < ../scripts/patch-gcc/config.gcc.patch
fi

# Begin building GCC
mkdir -p build-gcc
pushd build-gcc
../gcc-8.2.0/configure --target=$TARGET --prefix="$PREFIX" --enable-languages=c,c++ --with-newlib $CONFIGURATION_OPTIONS
unset LIBRARY_PATH
make $PARALLEL_MAKE all-gcc
make install-gcc
popd

# Build the C standard library
# Our freestanding kernel won't actually use this, but we need to build it in
# order to get libstdc++ to build. We won't actually use that, but we need to
# build it in order to get the headers that a compliant C++ compiler is required
# to provide in a freestanding environment.
mkdir -p build-newlib
pushd build-newlib
../newlib-master/configure --target=$TARGET --prefix="$PREFIX" $CONFIGURATION_OPTIONS
make $PARALLEL_MAKE
make install
popd

# Circle back and build the rest of GCC, including the C++ standard library
pushd build-gcc
make $PARALLEL_MAKE all
make install
popd
