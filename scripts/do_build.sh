#!/bin/sh
set -e
set -o pipefail
BUILD_DIR="$1"
cmake --build "$BUILD_DIR" || exit 1
