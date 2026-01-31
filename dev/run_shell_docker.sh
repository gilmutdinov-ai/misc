#!/bin/bash

source "$(dirname "$0")/common.sh"

#-v D:/SourceCodes/misc/data:/export/misc/data
#--network contentv1_network

MNT_DWN="C:/Users/Admin/Downloads/:/C_DWN/"

docker run -i $BAZEL_CACHE_MOUNT_OPT $BAZEL_MIRROR_MOUNT_OPT -v E:/SourceCodes/misc:/sources/misc -v E:/SourceCodes/misc/data:/export/misc/data -v $MNT_DWN  --privileged docker.io/library/misc-dev-ub2204 bash
