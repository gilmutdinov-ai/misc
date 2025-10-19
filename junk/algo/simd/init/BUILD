cc_binary(
    name = "simd_init",
    srcs = [
        "simd_init.cpp"
    ],
    deps = [
        "//junk/algo/simd/common"
    ],
    copts = [
        "-mavx2 -masm=att"
    ],
    linkopts = [],
    visibility = ["//visibility:public"]
)