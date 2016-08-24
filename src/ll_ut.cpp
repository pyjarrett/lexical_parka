#include <gtest/gtest.h>

#include "grammar.hpp"
#include "ll.hpp"
#include "streams.hpp"
#include "symbol.hpp"
using namespace context_free;

#include "sample_grammar_test_fixtures.hpp"

#include <algorithm>
#include <utility>


TEST(Predictive_Parser_Table_Test, Simple_Grammar) {
  Grammar grammar;
  grammar.set_alternatives("S"_sym, {"F"_sym | "("_sym + "S"_sym + "+"_sym + "F"_sym + ")"_sym});
  grammar.set_alternatives("F"_sym, {"a"_sym});

  Predictive_Parsing_Table parsing_table;
  ASSERT_TRUE(create_predictive_parsing_table(grammar, &parsing_table));

  ASSERT_TRUE(parsing_table[Symbol_Pair("S"_sym, "a"_sym)] == Production("S"_sym, {"F"_sym}));
  ASSERT_TRUE(parsing_table[Symbol_Pair("S"_sym, "("_sym)] == Production("S"_sym, {"("_sym + "S"_sym + "+"_sym + "F"_sym + ")"_sym}));
  ASSERT_TRUE(parsing_table[Symbol_Pair("F"_sym, {"a"_sym})] == Production("F"_sym, {"a"_sym}));
}

TEST_F(Non_Left_Recursive_Add_Multiply_Grammar_Test, Predictive_Parser_Table_Test) {
  Predictive_Parsing_Table parsing_table;
  ASSERT_TRUE(create_predictive_parsing_table(grammar, &parsing_table));

  ASSERT_EQ(parsing_table[Symbol_Pair("E"_sym, "id"_sym)], Production("E"_sym, {"T"_sym + "E'"_sym}));
  ASSERT_EQ(parsing_table[Symbol_Pair("T"_sym, "id"_sym)], Production("T"_sym, {"F"_sym + "T'"_sym}));
  ASSERT_EQ(parsing_table[Symbol_Pair("F"_sym, "id"_sym)], Production("F"_sym, {"id"_sym}));
  ASSERT_EQ(parsing_table[Symbol_Pair("E'"_sym, "+"_sym)], Production("E'"_sym, {"+"_sym + "T"_sym + "E'"_sym}));
  ASSERT_EQ(parsing_table[Symbol_Pair("T'"_sym, "+"_sym)], Production("T'"_sym, {Symbol::empty()}));
  ASSERT_EQ(parsing_table[Symbol_Pair("T'"_sym, "*"_sym)], Production("T'"_sym, {"*"_sym + "F"_sym + "T'"_sym}));
  ASSERT_EQ(parsing_table[Symbol_Pair("E"_sym, "("_sym)], Production("E"_sym, {"T"_sym + "E'"_sym}));
  ASSERT_EQ(parsing_table[Symbol_Pair("T"_sym, "("_sym)], Production("T"_sym, {"F"_sym + "T'"_sym}));
  ASSERT_EQ(parsing_table[Symbol_Pair("F"_sym, "("_sym)], Production("F"_sym, {"("_sym + "E"_sym + ")"_sym}));
  ASSERT_EQ(parsing_table[Symbol_Pair("E'"_sym, ")"_sym)], Production("E'"_sym, {Symbol::empty()}));
  ASSERT_EQ(parsing_table[Symbol_Pair("T'"_sym, ")"_sym)], Production("T'"_sym, {Symbol::empty()}));
  ASSERT_EQ(parsing_table[Symbol_Pair("E'"_sym, "$"_sym)], Production("E'"_sym, {Symbol::empty()}));
  ASSERT_EQ(parsing_table[Symbol_Pair("T'"_sym, "$"_sym)], Production("T'"_sym, {Symbol::empty()}));
}

TEST(Ambiguous_LL1_Grammar, Predictive_Parser_Table_Test) {
  Grammar grammar;
  grammar.set_alternatives("S"_sym, {"i"_sym + "E"_sym + "t"_sym + "S"_sym + "S'"_sym | "a"_sym});
  grammar.set_alternatives("S'"_sym, {"e"_sym + "S"_sym | Symbol::empty()});
  grammar.set_alternatives("E"_sym, {"b"_sym});

  Predictive_Parsing_Table parsing_table;
  ASSERT_FALSE(create_predictive_parsing_table(grammar, &parsing_table));
}


int main(int argc, char ** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
