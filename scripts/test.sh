#!/bin/sh
set -e

HOST_BUILD_DIR="build_host"

if [ ! -d "$HOST_BUILD_DIR" ]; then
    cmake -E make_directory "$HOST_BUILD_DIR"
    cmake -E chdir "$HOST_BUILD_DIR" cmake ..
fi

cmake --build "$HOST_BUILD_DIR"

cd build_host
ctest --output-on-failure
