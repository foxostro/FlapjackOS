#!/bin/sh
# Download and build a GCC cross compiler for Flapjack OS.
# See also <https://wiki.osdev.org/GCC_Cross-Compiler>
set -euxo pipefail

TARGET=$1

mkdir -p build_toolchain
cd build_toolchain

if [ ! -f gcc-8.2.0.tar.gz ]; then
   wget http://www.netgull.com/gcc/releases/gcc-8.2.0/gcc-8.2.0.tar.gz
fi

if [ ! -f newlib-master.zip ]; then
   wget -nc -O newlib-master.zip https://github.com/bminor/newlib/archive/master.zip
fi

if [ ! -d gcc-8.2.0 ]; then
	tar xzvf gcc-8.2.0.tar.gz

	# Patch GCC
	if [[ $TARGET == "x86_64-elf" ]]; then
		mkdir -p ./gcc-8.2.0/gcc/config/i386/
		cp ../scripts/patch-gcc/t-x86_64-elf ./gcc-8.2.0/gcc/config/i386/
		patch -p0 ./gcc-8.2.0/gcc/config.gcc < ../scripts/patch-gcc/config.gcc.patch
	fi
fi

if [ ! -d newlib-master ]; then
    unzip -qo newlib-master.zip
fi
