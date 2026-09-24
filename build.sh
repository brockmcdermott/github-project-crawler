#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="${BUILD_DIR:-build}"
BUILD_TYPE="${BUILD_TYPE:-Debug}"
CXX="${CXX:-clang++}"

if ! command -v cmake >/dev/null 2>&1; then
    echo "Error: cmake was not found on PATH. Install CMake and try again." >&2
    exit 1
fi

if ! command -v "$CXX" >/dev/null 2>&1; then
    echo "Error: C++ compiler '$CXX' was not found on PATH." >&2
    exit 1
fi

cmake_args=(
    -S .
    -B "$BUILD_DIR"
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
    -DCMAKE_CXX_COMPILER="$CXX"
)

if [[ -z "${CMAKE_PREFIX_PATH:-}" ]] && command -v brew >/dev/null 2>&1; then
    cmake_args+=("-DCMAKE_PREFIX_PATH=$(brew --prefix)")
fi

cmake "${cmake_args[@]}"

cmake --build "$BUILD_DIR"
