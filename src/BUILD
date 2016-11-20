load("//tools:gtest.bzl", "gtest_unit")

gtest_unit(
  name = "symbol",
  components = ["streams"],
  srcs = ["string.hpp"],
)

gtest_unit(
  name = "grammar",
  components = ["symbol",
                "streams"],
  srcs = ["sample_grammar_test_fixtures.hpp",
          "string.hpp"],
)

gtest_unit(
  name = "ll",
  components = ["ast",
                "symbol",
                "streams",
                "grammar",
                "lexer"],
  srcs = ["regex.hpp",
          "sample_grammar_test_fixtures.hpp",
          "string.hpp"],
)

gtest_unit(
  name = "lexer",
  components = ["symbol",
                "streams"],
  srcs = ["regex.hpp",
          "string.hpp"],
)
