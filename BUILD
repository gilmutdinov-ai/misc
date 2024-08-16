package(
    default_visibility = ["//visibility:public"]
)

cc_library(
    name = "misc",
    srcs = [
        "Config.cpp",
        "Time.cpp",
        "Bazel.cpp",
        "Js.cpp",
        "Strings.cpp",
        "Vectors.cpp",
        "MergeQueue.h",
        "Base64.cpp",
        "LvlDb.cpp"
    ],
    hdrs = [
        "Log.h",
        "Config.h",
        "Time.h",
        "Js.h",
        "Bazel.h",
        "Strings.h",
        "Vectors.h",
        "Base64.h",
        "LvlDb.h"
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@jsoncpp//:jsoncpp",
        "@date//:date",
        "@concurrentqueue//:concurrentqueue"
    ],
    copts = [
        "-Isrc/",
        "-Iexternal/jsoncpp"
    ],
    linkopts = ["-lleveldb"]
)