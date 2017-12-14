#!/bin/sh
set -e
./scripts/build.sh
cd build_host
ctest --output-on-failure
