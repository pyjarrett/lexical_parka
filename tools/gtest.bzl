def gtest_unit(name, deps=[], components=[], srcs=[], 
              copts=[], size="small", args=[],
              **kwargs):
  components = components + [name]
  # Build list of components
  for x in components:
    srcs = srcs + [x + ".hpp", x + ".cpp"]

  native.cc_test(
    name = name + "_test",
    deps = deps + ["@gtest//:lib"],
    srcs = srcs + [name + "_ut.cpp"],
    copts = copts + ["-Wall", "-Wextra", "-Iexternal/gtest/include", "-std=c++14"],
    size = size,

    # Ensure we get colorized output from bazel.
    args = args + ["--gtest_color=yes"],
    **kwargs
  )
