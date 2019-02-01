#!/bin/sh
set -eu
rm -rf "CMakeCache.txt" \
       "CMakeFiles/" \
       "CMakeScripts/" \
       "CTestTestfile.cmake" \
       "Common/CTestTestfile.cmake" \
       "Common/FlapjackOS.build/" \
       "Common/cmake_install.cmake" \
       "FlapjackOS.build/" \
       "Kernel/CMakeScripts/" \
       "Kernel/CTestTestfile.cmake" \
       "Kernel/FlapjackOS.build/" \
       "Kernel/cmake_install.cmake" \
       "Malloc/CTestTestfile.cmake" \
       "Malloc/FlapjackOS.build/" \
       "Malloc/cmake_install.cmake" \
       "$FLAPJACKOS_BUILD_DIR/build/" \
       "cmake_install.cmake" \
