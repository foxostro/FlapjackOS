#!/bin/sh
set -e
set -o pipefail
BUILD_DIR="$1"
cmake --build "$BUILD_DIR" \
    | sed -E 's/^\.\.\/\.\.\/(.*):/\1:/' \
    | GREP_COLOR='01;36' grep --color=always -E '^.*:\d+:\d+:|$' \
    | GREP_COLOR='01;31' grep --color=always -E 'error:|$' \
    | GREP_COLOR='01;32' grep --color=always -E '\w*~*\^~*|$' \
  || exit 1
