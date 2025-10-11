workspace(name = "contentv1_workspace")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "catch2",
    strip_prefix = "Catch2-3.11.0",
    urls = ["https://github.com/catchorg/Catch2/archive/refs/tags/v3.11.0.tar.gz"],
)

new_local_repository(
    name = "cpp-httplib",
    path = "./contribs/cpp-httplib/",
    build_file_content = """
package(default_visibility = ["//visibility:public"])
cc_library(
    name = "cpp-httplib",
    srcs = glob([
    ]),
    hdrs = glob([
        "httplib.h"
    ]),
    includes = [],
    copts = [],
    visibility = ["//visibility:public"]
)
"""
)

# simdjson for parsing streams from kafka, etc
new_local_repository(
    name = "simdjson",
    path = "/usr/include/",
    build_file_content = """
package(default_visibility = ["//visibility:public"])
cc_library(
    name = "simdjson",
    hdrs = glob(["simdjson.h"]),
    includes = ["./"],
    linkopts = ["-lsimdjson"],
    visibility = ["//visibility:public"],
)
"""
)

new_local_repository(
    name = "librdkafka",
    path = "/usr/include/librdkafka/",
    build_file_content = """
package(default_visibility = ["//visibility:public"])
cc_library(
    name = "librdkafka",
    hdrs = glob(["**/*.h"]),
    linkopts = ["-lrdkafka++"],
    visibility = ["//visibility:public"],
)
"""
)
