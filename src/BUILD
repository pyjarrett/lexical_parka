load("//tools:gtest.bzl", "gtest_unit")

gtest_unit(
    name = "symbol",
    components = ["streams"],
)

gtest_unit(
  name = "grammar",
  components = ["symbol",
                "streams"],
  srcs = ["sample_grammar_test_fixtures.hpp"],
)

gtest_unit(
  name = "ll",
  components = ["symbol",
                "streams",
                "grammar",
                "lexer"],
  srcs = ["sample_grammar_test_fixtures.hpp"],
)

gtest_unit(
  name = "lexer",
)
