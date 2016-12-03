#!/bin/sh
rm -rf CMakeFiles CMakeCache.txt
cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=i386-elf.toolchain.cmake .
