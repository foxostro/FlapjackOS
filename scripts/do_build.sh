#!/bin/sh
set -o pipefail
BUILD_DIR="$1"
OPTS=-j8
cmake --build "$BUILD_DIR" -- $OPTS || echo "ERROR: Build failed for \"$BUILD_DIR\"."
