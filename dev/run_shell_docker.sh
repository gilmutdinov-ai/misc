#!/bin/bash

source "$(dirname "$0")/common.sh"

#-v D:/SourceCodes/misc/data:/export/misc/data
#--network contentv1_network

docker run -i $BAZEL_CACHE_MOUNT_OPT $BAZEL_MIRROR_MOUNT_OPT -v D:/SourceCodes/misc:/sources/misc --privileged docker.io/library/misc-dev-ub2204 bash
