cc_inc_library(
    name = "gtest_inc",
    hdrs = glob(["include/**/*.h"]),
    prefix = "include/gtest",
    deps = [":gtest_lib"],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "lib",
    srcs = glob(
        ["src/*.cc",
        "src/*.h"],
        exclude = ["src/gtest-all.cc"],
    ),
    hdrs = glob([
        "include/**/*.h",
        "src/*.h"
    ]),
    includes = [
        "include",
        ""
    ],
    linkopts = ["-lpthread"],
    visibility = ["//visibility:public"],
)
