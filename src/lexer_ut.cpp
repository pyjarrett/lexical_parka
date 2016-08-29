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


int main(int argc, char ** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
