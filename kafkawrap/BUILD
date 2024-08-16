package(
    default_visibility = ["//visibility:public"]
)

cc_library(
    name = "kafkawrap",
    srcs = [
        "KafkaCommon.cpp",
        "KafkaReader.cpp",
        "KafkaReaderMock.cpp",
        "KafkaWriter.cpp",
        "KafkaWriterMock.cpp",
        "KafkaStaticSim.cpp"
    ],
    hdrs = [
        "KafkaCommon.h",
        "KafkaReader.h",
        "KafkaReaderMock.h",
        "KafkaWriter.h",
        "KafkaWriterMock.h",
        "KafkaStaticSim.h"
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@librdkafka//:librdkafka",
        "@simdjson//:simdjson"
    ],
    copts = [
        "-Isrc/",
        "-Iexternal/librdkafka",
        "-Iexternal/simdjson"
    ]
)