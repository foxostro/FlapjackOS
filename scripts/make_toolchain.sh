#!/bin/sh
# Download and build a toolchain for building Flapjack OS.
set -euxo pipefail
TARGET=$1
echo "Requesting root in order to install dependencies from MacPorts."
sudo port install wget gmp mpfr libmpc libiconv
echo "Downloading and preparing packages..."
./scripts/download_binutils.sh
./scripts/download_gcc.sh "$TARGET"
echo "Building binutils..."
./scripts/make_binutils.sh "$TARGET"
echo "Building GCC..."
./scripts/make_gcc.sh "$TARGET"
echo "Done."
