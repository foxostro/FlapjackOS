#!/bin/sh
set -eu

PROJECT_DIR="$1"; shift
BUILD_DIR="$1"; shift
OTHER_OPTIONS="$@"

# If the build directory already exists then assume no configuration is needed.
if [ -d "$BUILD_DIR" ]; then
    exit 0
fi

# Use the Ninja generator if the ninja tool is available.
GENERATOR=""
echo "Checking for Ninja..."
if [ -z `which ninja` ]; then
    echo "ninja is not in the path."
else
    echo `which ninja`
    GENERATOR="-GNinja"
fi

# Use ccache if it is available.
CCACHE_LAUNCHER=""
echo "Checking for ccache..."
if [ -z `which ccache` ]; then
    echo "ccache is not in the path."
else
    echo `which ccache`
    CCACHE_LAUNCHER="-DCMAKE_CXX_COMPILER_LAUNCHER=ccache"
fi

cmake -E make_directory "$BUILD_DIR"
cmake -E chdir "$BUILD_DIR" cmake "$PROJECT_DIR" $GENERATOR $CCACHE_LAUNCHER $OTHER_OPTIONS
