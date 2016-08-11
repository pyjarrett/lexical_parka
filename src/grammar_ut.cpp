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

TEST(Grammar_Test, Add_Multiple_Alternative_Production) {
  Grammar grammar;
  grammar["A"_sym] = {"B"_sym + "C"_sym | "D"_sym | "E"_sym + "F"_sym};
  EXPECT_EQ("B C | D | E F", as_string(grammar["A"_sym]));
}

TEST(Grammar_Has_Empty_Production_Test, Single_Empty_Symbol) {
  Grammar grammar;
  grammar["A"_sym] = {Symbol::empty()};
  ASSERT_TRUE(grammar.has_empty_production("A"_sym));
}

TEST(Grammar_Has_Empty_Production_Test, Single_Terminal) {
  Grammar grammar;
  grammar["A"_sym] = {"B"_sym};
  ASSERT_FALSE(grammar.has_empty_production("A"_sym));
}

TEST(Grammar_Has_Empty_Production_Test, Terminal_Or_Empty) {
  Grammar grammar;
  grammar["A"_sym] = {"B"_sym | Symbol::empty()};
  ASSERT_TRUE(grammar.has_empty_production("A"_sym));
}

TEST(Grammar_Has_Empty_Production_Test, Two_Level) {
  Grammar grammar;
  grammar["A"_sym] = {"B"_sym};
  grammar["B"_sym] = {Symbol::empty()};
  ASSERT_TRUE(grammar.has_empty_production("A"_sym));
  ASSERT_TRUE(grammar.has_empty_production("B"_sym));
}

TEST(Grammar_Has_Empty_Production_Test, Simple_Circular) {
  Grammar grammar;
  grammar["A"_sym] = {"B"_sym};
  grammar["B"_sym] = {"A"_sym};
  EXPECT_FALSE(grammar.has_empty_production("A"_sym));
  EXPECT_FALSE(grammar.has_empty_production("B"_sym));
}

TEST(Grammar_Has_Empty_Production_Test, Circular_With_Empty) {
  Grammar grammar;
  grammar["A"_sym] = {"B"_sym | Symbol::empty()};
  grammar["B"_sym] = {"A"_sym | Symbol::empty()};
  grammar["C"_sym] = {"D"_sym};
  grammar["D"_sym] = {"C"_sym};
  grammar["E"_sym] = {"C"_sym};
  ASSERT_TRUE(grammar.has_empty_production("A"_sym));
  ASSERT_TRUE(grammar.has_empty_production("B"_sym));
  ASSERT_FALSE(grammar.has_empty_production("C"_sym));
  ASSERT_FALSE(grammar.has_empty_production("D"_sym));
  ASSERT_FALSE(grammar.has_empty_production("E"_sym));
}

TEST(Grammar_Has_Empty_Production_Test, Multiple_Symbol_Strings) {
  Grammar grammar;
  grammar["A"_sym] = {"B"_sym | Symbol::empty()};
  grammar["B"_sym] = {"B"_sym + "A"_sym | "A"_sym | Symbol::empty()};
  grammar["C"_sym] = {"D"_sym | "D"_sym + "D"_sym | "D"_sym + "B"_sym};
  grammar["D"_sym] = {"C"_sym | "C"_sym + "A"_sym};
  grammar["E"_sym] = {"C"_sym | "C"_sym + "A"_sym};
  ASSERT_TRUE(grammar.has_empty_production("A"_sym));
  ASSERT_TRUE(grammar.has_empty_production("B"_sym));
  ASSERT_FALSE(grammar.has_empty_production("C"_sym));
  ASSERT_FALSE(grammar.has_empty_production("D"_sym));
  ASSERT_FALSE(grammar.has_empty_production("E"_sym));
}


int main(int argc, char ** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
