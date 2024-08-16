cc_library(
    name = "httpcli",
    srcs = [
        "http_client.cpp"
    ],
    hdrs = [
        "http_client.h",
        "http_client.impl"
    ],
    visibility = ["//visibility:public"],
    #deps = ["@curl//:curl"],
    copts = ["-Isrc"],
    linkopts = ["-lcurl"]
)