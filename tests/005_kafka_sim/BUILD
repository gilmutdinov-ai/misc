
cc_test(
    name = "021_kafka_sim",
    srcs = ["ut_kafka_sim.cpp"],
    copts = ["-Isrc/"],
    deps = [
        "@catch2//:catch2_main",
        "@ada//:ada",
        "//src/misc/kafkawrap/ut"
    ],
    visibility = ["//visibility:public"]
)
