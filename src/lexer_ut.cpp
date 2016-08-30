#include <gtest/gtest.h>

#include "lexer.hpp"
using namespace parka;

#include <iostream>


TEST(Lexer_Test, Single_Integer) {
  Lexer lexer;
  lexer.register_pattern_for_token("(-)?[0-9]+", "integer");
  lexer.lex("123");
  ASSERT_TRUE(lexer.has_next_token());
  EXPECT_EQ("123", lexer.next_token().lexeme);
}


TEST(Lexer_Test, Multiple_Integers) {
  Lexer lexer;
  lexer.register_pattern_for_token("(-)?[0-9]+", "integer");
  lexer.lex("123 456 789\n234 -345");
  ASSERT_TRUE(lexer.has_next_token());
  EXPECT_EQ("123", lexer.next_token().lexeme);

  ASSERT_TRUE(lexer.has_next_token());
  EXPECT_EQ("456", lexer.next_token().lexeme);

  ASSERT_TRUE(lexer.has_next_token());
  EXPECT_EQ("789", lexer.next_token().lexeme);

  ASSERT_TRUE(lexer.has_next_token());
  EXPECT_EQ("234", lexer.next_token().lexeme);

  ASSERT_TRUE(lexer.has_next_token());
  EXPECT_EQ("-345", lexer.next_token().lexeme);
}


TEST(Lexer_Test, Keyword_Identifier_Recognition) {
  Lexer lexer;
  lexer.register_keyword("for");
  lexer.register_keyword("in");
  lexer.register_pattern_for_token("[a-zA-Z_][a-zA-Z_0-9]*", "identifier");

  lexer.lex("for x in list");

  Token tk;

  ASSERT_TRUE(lexer.has_next_token());
  tk = lexer.next_token();
  EXPECT_EQ("for", tk.symbol_name);
  EXPECT_EQ("for", tk.lexeme);

  ASSERT_TRUE(lexer.has_next_token());
  tk = lexer.next_token();
  EXPECT_EQ("identifier", tk.symbol_name);
  EXPECT_EQ("x", tk.lexeme);

  ASSERT_TRUE(lexer.has_next_token());
  tk = lexer.next_token();
  EXPECT_EQ("in", tk.symbol_name);
  EXPECT_EQ("in", tk.lexeme);

  ASSERT_TRUE(lexer.has_next_token());
  tk = lexer.next_token();
  EXPECT_EQ("identifier", tk.symbol_name);
  EXPECT_EQ("list", tk.lexeme);

  ASSERT_FALSE(lexer.has_next_token());
}


int main(int argc, char ** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
