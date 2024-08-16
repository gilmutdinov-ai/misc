package(
    default_visibility = ["//visibility:public"]
)

cc_library(
    name = "ut",
    srcs = [
        "ChWriterTester.cpp"
    ],
    hdrs = [
        "ChWriterTester.h"
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@cpp-httplib//:cpp-httplib",
        "@catch2//:catch2_main",
        "//src/misc/ch"
    ],
    copts = [
        "-Isrc/"
    ]
)