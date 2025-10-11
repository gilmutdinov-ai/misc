
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
        "//kafkawrap",
        #"//src/tests/common"
    ],
    copts = [
        "-Isrc/"
    ]
)