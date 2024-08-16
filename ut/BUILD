package(
    default_visibility = ["//visibility:public"]
)
cc_library(
    name = "ut",
    srcs = [
        "MergeQueueTester.cpp",
        "MiscTester.cpp"
    ],
    hdrs = [
        "MergeQueueTester.h",
        "MiscTester.h"
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@catch2//:catch2_main",
        "//src/misc",
        "//src/tests/common"
    ],
    copts = [
        "-Isrc/"
    ]
)