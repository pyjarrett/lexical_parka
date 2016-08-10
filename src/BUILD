cc_test(
  name = "symbol_test",
  deps = ["@gtest//:lib"],
  srcs = ["symbol.cpp",
         "symbol.hpp",
         "streams.hpp",
         "streams.cpp",
         "symbol_ut.cpp"],
  copts = ["-Iexternal/gtest/include"],
  size = "small",
  visibility = ["//src:__pkg__"],
  args = ["--gtest_color=yes"]
)


cc_test(
  name = "grammar_test",
  deps = ["@gtest//:lib"],
  srcs = ["symbol.hpp",
          "symbol.cpp",
          "streams.hpp",
          "streams.cpp",
          "grammar.hpp",
          "grammar.cpp",
          "grammar_ut.cpp"],
  copts = ["-Iexternal/gtest/include"],
  size = "small",
  visibility = ["//src:__pkg__"],
  args = ["--gtest_color=yes"]
)
