#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="${BUILD_DIR:-build}"
BUILD_TYPE="${BUILD_TYPE:-Debug}"
CC="${CC:-clang}"
CXX="${CXX:-clang++}"

if ! command -v cmake >/dev/null 2>&1; then
    echo "Error: cmake was not found on PATH. Install CMake and try again." >&2
    exit 1
fi

if ! command -v "$CC" >/dev/null 2>&1; then
    echo "Error: C compiler '$CC' was not found on PATH." >&2
    exit 1
fi

if ! command -v "$CXX" >/dev/null 2>&1; then
    echo "Error: C++ compiler '$CXX' was not found on PATH." >&2
    exit 1
fi

cmake -S . -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX"

cmake --build "$BUILD_DIR"
