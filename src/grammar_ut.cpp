#include <gtest/gtest.h>

#include <algorithm>
#include "grammar.hpp"
#include "streams.hpp"
#include "symbol.hpp"
using namespace context_free;

class Add_Multiply_Grammar_Test : public ::testing::Test {
protected:
  Grammar grammar;

  virtual void SetUp() {
    grammar.set_production("E"_sym, {"E"_sym + "+"_sym + "T"_sym | "T"_sym});
    grammar.set_production("T"_sym, {"T"_sym + "*"_sym + "F"_sym | "F"_sym});
    grammar.set_production("F"_sym, {"("_sym + "E"_sym + ")"_sym | "id"_sym});
  }

  virtual void TearDown() {}
};

class Non_Left_Recursive_Add_Multiply_Grammar_Test : public ::testing::Test {
protected:
  Grammar grammar;

  virtual void SetUp() {
    grammar.set_production("E"_sym, {"T"_sym + "E'"_sym});
    grammar.set_production("E'"_sym, {"+"_sym + "T"_sym + "E'"_sym | Symbol::empty()});
    grammar.set_production("T"_sym, {"F"_sym + "T'"_sym});
    grammar.set_production("T'"_sym, {"*"_sym + "F"_sym + "T'"_sym | Symbol::empty()});
    grammar.set_production("F"_sym, {"("_sym + "E"_sym + ")"_sym | "id"_sym});
  }

  virtual void TearDown() {}
};

////////////////////////////////////////////////////////////////////////////////
// Basic production assignment tests.
////////////////////////////////////////////////////////////////////////////////
TEST(Grammar_Test, Add_Empty_Production) {
  Grammar grammar;
  grammar.set_production("A"_sym, {Symbol::empty()});
  EXPECT_EQ("empty", as_string(grammar["A"_sym]));
  EXPECT_EQ("A"_sym, grammar.start_symbol());
}

TEST(Grammar_Test, Add_Multiple_Alternative_Production) {
  Grammar grammar;
  grammar.set_production("A"_sym, {"B"_sym + "C"_sym | "D"_sym | "E"_sym + "F"_sym});
  EXPECT_EQ("B C | D | E F", as_string(grammar["A"_sym]));
  EXPECT_EQ("A"_sym, grammar.start_symbol());
}

////////////////////////////////////////////////////////////////////////////////
// Start symbol tests
////////////////////////////////////////////////////////////////////////////////
TEST(Grammar_Start_Symbol_Test, Add_Multiple_Productions) {
  Grammar grammar;
  grammar.set_production("Z"_sym, {"B"_sym + "D"_sym});
  grammar.set_production("A"_sym, {"C"_sym + "D"_sym});
  grammar.set_production("B"_sym, {"C"_sym + "D"_sym});
  EXPECT_EQ("Z"_sym, grammar.start_symbol());
}

////////////////////////////////////////////////////////////////////////////////
// Has_Empty_Production_Test
////////////////////////////////////////////////////////////////////////////////
TEST(Grammar_Has_Empty_Production_Test, Single_Empty_Symbol) {
  Grammar grammar;
  grammar.set_production("A"_sym, {Symbol::empty()});
  ASSERT_TRUE(grammar.has_empty_production("A"_sym));
}

TEST(Grammar_Has_Empty_Production_Test, Single_Terminal) {
  Grammar grammar;
  grammar.set_production("A"_sym, {"B"_sym});
  ASSERT_FALSE(grammar.has_empty_production("A"_sym));
}

TEST(Grammar_Has_Empty_Production_Test, Terminal_Or_Empty) {
  Grammar grammar;
  grammar.set_production("A"_sym, {"B"_sym | Symbol::empty()});
  ASSERT_TRUE(grammar.has_empty_production("A"_sym));
}

TEST(Grammar_Has_Empty_Production_Test, Two_Level) {
  Grammar grammar;
  grammar.set_production("A"_sym, {"B"_sym});
  grammar.set_production("B"_sym, {Symbol::empty()});
  ASSERT_TRUE(grammar.has_empty_production("A"_sym));
  ASSERT_TRUE(grammar.has_empty_production("B"_sym));
}

TEST(Grammar_Has_Empty_Production_Test, Simple_Circular) {
  Grammar grammar;
  grammar.set_production("A"_sym, {"B"_sym});
  grammar.set_production("B"_sym, {"A"_sym});
  EXPECT_FALSE(grammar.has_empty_production("A"_sym));
  EXPECT_FALSE(grammar.has_empty_production("B"_sym));
}

TEST(Grammar_Has_Empty_Production_Test, Circular_With_Empty) {
  Grammar grammar;
  grammar.set_production("A"_sym, {"B"_sym | Symbol::empty()});
  grammar.set_production("B"_sym, {"A"_sym | Symbol::empty()});
  grammar.set_production("C"_sym, {"D"_sym});
  grammar.set_production("D"_sym, {"C"_sym});
  grammar.set_production("E"_sym, {"C"_sym});
  ASSERT_TRUE(grammar.has_empty_production("A"_sym));
  ASSERT_TRUE(grammar.has_empty_production("B"_sym));
  ASSERT_FALSE(grammar.has_empty_production("C"_sym));
  ASSERT_FALSE(grammar.has_empty_production("D"_sym));
  ASSERT_FALSE(grammar.has_empty_production("E"_sym));
}

TEST(Grammar_Has_Empty_Production_Test, Multiple_Symbol_Strings) {
  Grammar grammar;
  grammar.set_production("A"_sym, {"B"_sym | Symbol::empty()});
  grammar.set_production("B"_sym, {"B"_sym + "A"_sym | "A"_sym | Symbol::empty()});
  grammar.set_production("C"_sym, {"D"_sym | "D"_sym + "D"_sym | "D"_sym + "B"_sym});
  grammar.set_production("D"_sym, {"C"_sym | "C"_sym + "A"_sym});
  grammar.set_production("E"_sym, {"C"_sym | "C"_sym + "A"_sym});
  ASSERT_TRUE(grammar.has_empty_production("A"_sym));
  ASSERT_TRUE(grammar.has_empty_production("B"_sym));
  ASSERT_FALSE(grammar.has_empty_production("C"_sym));
  ASSERT_FALSE(grammar.has_empty_production("D"_sym));
  ASSERT_FALSE(grammar.has_empty_production("E"_sym));
}

////////////////////////////////////////////////////////////////////////////////
// Terminal checks
////////////////////////////////////////////////////////////////////////////////
TEST(Grammar_Terminal_Test, Terminal_Test) {
  Grammar grammar;
  grammar.set_production("A"_sym, {"B"_sym});
  grammar.set_production("B"_sym, {Symbol::empty()});
  grammar.set_production("C"_sym, {"A"_sym});

  ASSERT_FALSE(grammar.is_terminal("A"_sym));
  ASSERT_FALSE(grammar.is_terminal("B"_sym));
  ASSERT_FALSE(grammar.is_terminal("C"_sym));
  ASSERT_TRUE(grammar.is_terminal("D"_sym));
}

////////////////////////////////////////////////////////////////////////////////
// FIRST(X) TESTS
////////////////////////////////////////////////////////////////////////////////
TEST_F(Add_Multiply_Grammar_Test, First_Test) {
  ASSERT_EQ(grammar.first("E"_sym), Symbol_Set({"("_sym, "id"_sym}));
  ASSERT_EQ(grammar.first("T"_sym), Symbol_Set({"("_sym, "id"_sym}));
  ASSERT_EQ(grammar.first("F"_sym), Symbol_Set({"("_sym, "id"_sym}));
  ASSERT_EQ(grammar.first("*"_sym), Symbol_Set({"*"_sym}));
  ASSERT_EQ(grammar.first("+"_sym), Symbol_Set({"+"_sym}));
  ASSERT_EQ(grammar.first(")"_sym), Symbol_Set({")"_sym}));
  ASSERT_EQ(grammar.first("("_sym), Symbol_Set({"("_sym}));
}

TEST_F(Non_Left_Recursive_Add_Multiply_Grammar_Test, First_Test) {
  ASSERT_TRUE(grammar.first("F"_sym) == Symbol_Set({"("_sym, "id"_sym}));
  ASSERT_TRUE(grammar.first("T"_sym) == Symbol_Set({"("_sym, "id"_sym}));
  ASSERT_TRUE(grammar.first("E"_sym) == Symbol_Set({"("_sym, "id"_sym}));
  ASSERT_TRUE(grammar.first("E'"_sym) == Symbol_Set({"+"_sym, Symbol::empty()}));
  ASSERT_TRUE(grammar.first("T'"_sym) == Symbol_Set({"*"_sym, Symbol::empty()}));
}

TEST_F(Non_Left_Recursive_Add_Multiply_Grammar_Test, First_String_Test) {
  ASSERT_EQ(grammar.first("E'"_sym + "T'"_sym),
      Symbol_Set({"+"_sym, "*"_sym, Symbol::empty()}));
  ASSERT_EQ(grammar.first("E'"_sym + "T'"_sym + "F"_sym),
      Symbol_Set({"+"_sym, "*"_sym, "("_sym, "id"_sym}));
}


////////////////////////////////////////////////////////////////////////////////
// FOLLOW(A) Tests
////////////////////////////////////////////////////////////////////////////////
TEST(Follow_Tests, Endmarker_Follows_Start_Symbol) {
  Grammar grammar;
  grammar.set_production("S"_sym, {"A"_sym});
  EXPECT_EQ(grammar.follow("S"_sym), Symbol_Set({Symbol::right_end_marker()}));
}


TEST_F(Non_Left_Recursive_Add_Multiply_Grammar_Test, Follow_Test) {
  EXPECT_EQ(grammar.follow("E"_sym), Symbol_Set({")"_sym, Symbol::right_end_marker()}));
  EXPECT_EQ(grammar.follow("E'"_sym), Symbol_Set({")"_sym, Symbol::right_end_marker()}));
  EXPECT_EQ(grammar.follow("T"_sym),
    Symbol_Set({"+"_sym, ")"_sym, Symbol::right_end_marker()}));
  EXPECT_EQ(grammar.follow("T'"_sym),
    Symbol_Set({"+"_sym, ")"_sym, Symbol::right_end_marker()}));
  EXPECT_EQ(grammar.follow("F"_sym),
    Symbol_Set({"*"_sym, "+"_sym, ")"_sym, Symbol::right_end_marker()}));
}

int main(int argc, char ** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
