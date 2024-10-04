#!/bin/bash
source "$(dirname "$0")/common.sh"

set -e

if [ -z "$1" ]
  then
    echo "Specify bin path ex: //junk/cpp/000_hello"
    exit 1
fi

BIN_PATH="$1"

CC=clang bazel build $BAZEL_CACHE_OPT $BAZEL_REPO_CACHE_OPT $BAZEL_REGISTRY_OPT --symlink_prefix $BAZEL_SYMLINK_PREFIX --experimental_enable_bzlmod --cxxopt=-std=c++20 --copt="-g"  --strip=never $BIN_PATH
