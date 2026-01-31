source "$(dirname "$0")/common.sh"

set -e

CC=clang $BAZEL_BIN test $BAZEL_CACHE_OPT $BAZEL_REPO_CACHE_OPT $BAZEL_REGISTRY_OPT --symlink_prefix $BAZEL_SYMLINK_PREFIX --enable_bzlmod --enable_workspace --cxxopt=-std=c++20 --copt="-g"  --strip=never //tests:tests