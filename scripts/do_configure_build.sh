#!/bin/sh
set -e

BUILD_DIR="$1"; shift
OTHER_OPTIONS="$@"

# If the build directory already exists then assume no configuration is needed.
if [ -d "$BUILD_DIR" ]; then
    exit 0
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
cmake -E chdir "$BUILD_DIR" cmake ../.. -G"Unix Makefiles" $CCACHE_LAUNCHER $OTHER_OPTIONS
