#include <gtest/gtest.h>

#include <algorithm>
#include "grammar.hpp"
#include "streams.hpp"
#include "symbol.hpp"
using namespace context_free;

TEST(Grammar_Test, Add_Empty_Production) {
  Grammar grammar;
  grammar["A"_sym] = {Symbol::empty()};
  EXPECT_EQ("empty", as_string(grammar["A"_sym]));
}

int main(int argc, char ** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
