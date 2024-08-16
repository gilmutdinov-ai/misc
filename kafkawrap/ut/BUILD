
cc_library(
    name = "ut",
    srcs = [
        "KafkaStaticSimTester.cpp"
    ],
    hdrs = [
        "KafkaStaticSimTester.h"
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@catch2//:catch2_main",
        "//src/kafkawrap",
        "//src/tests/common"
    ],
    copts = [
        "-Isrc/"
    ]
)