#include <gtest/gtest.h>

#include "lexer.hpp"
using namespace parka;

#include <iostream>


class Lexer_Test : public ::testing::Test {
protected:
  void
  ASSERT_FIND_EXPECTED_TOKENS(
    std::vector<std::pair<std::string, std::string>> & expected,
    Lexer & lexer)
  {
    Token tk;
    for (auto name_lexeme : expected) {
      ASSERT_TRUE(lexer.has_next_token());
      tk = lexer.next_token();
      EXPECT_EQ(name_lexeme.first, tk.symbol_name);
      EXPECT_EQ(name_lexeme.second, tk.lexeme);
    }
    ASSERT_FALSE(lexer.has_next_token());
  }
};


TEST_F(Lexer_Test, Single_Integer) {
  Lexer lexer;
  lexer.register_pattern_for_token("(-)?([1-9][0-9]*|0)", "integer");
  lexer.lex("123");
  ASSERT_TRUE(lexer.has_next_token());
  EXPECT_EQ("123", lexer.next_token().lexeme);
}


TEST_F(Lexer_Test, Multiple_Integers) {
  Lexer lexer;
  lexer.register_pattern_for_token("(-)?([1-9][0-9]*|0)", "integer");
  lexer.lex("123 456 789\n234 -345");

  std::vector<std::pair<std::string, std::string>> expected = {
    {"integer", "123"},
    {"integer", "456"},
    {"integer", "789"},
    {"integer", "234"},
    {"integer", "-345"}};
  ASSERT_FIND_EXPECTED_TOKENS(expected, lexer);
}


TEST_F(Lexer_Test, Keyword_Identifier_Recognition) {
  Lexer lexer;
  lexer.register_keyword("for");
  lexer.register_keyword("in");
  lexer.register_pattern_for_token("[a-zA-Z_][a-zA-Z_0-9]*", "identifier");

  lexer.lex("for x in list");

  std::vector<std::pair<std::string, std::string>> expected = {
    {"for", "for"},
    {"identifier", "x"},
    {"in", "in"},
    {"identifier", "list"}};
  ASSERT_FIND_EXPECTED_TOKENS(expected, lexer);
}


TEST_F(Lexer_Test, Keywords_As_Part_Of_Identifiers) {
  Lexer lexer;
  lexer.register_keyword("for");
  lexer.register_keyword("in");
  lexer.register_pattern_for_token("[a-zA-Z_][a-zA-Z_0-9]*", "identifier");

  lexer.lex(R"(
  foreach mine in inner
  )");

  std::vector<std::pair<std::string, std::string>> expected = {
    {"identifier", "foreach"}, {"identifier", "mine"}, {"in", "in"}, {"identifier", "inner"}};
  ASSERT_FIND_EXPECTED_TOKENS(expected, lexer);
}


TEST_F(Lexer_Test, Single_Function_Call) {
  Lexer lexer;
  lexer.register_pattern_for_token("[a-zA-Z_][a-zA-Z_0-9]*", "identifier");
  lexer.register_pattern_for_token("[(]", "(");
  lexer.register_pattern_for_token("[)]", ")");

  lexer.lex("cos(x)");
  std::vector<std::pair<std::string, std::string>> expected = {
    {"identifier", "cos"}, {"(", "("}, {"identifier", "x"}, {")", ")"}};
  ASSERT_FIND_EXPECTED_TOKENS(expected, lexer);
}


TEST_F(Lexer_Test, Quoted_String) {
  Lexer lexer;
  lexer.register_pattern_for_token("\"([^\"\\\\]|(\\\\.))*\"", "quoted_string");
  lexer.lex(R"(
  "word"
  "multiple words"
  "words with something \"in quotes\""
  "words
  including a newline."
  "a really pathological case \\"
  )");
  std::vector<std::pair<std::string, std::string>> expected = {
    {"quoted_string", "\"word\""},
    {"quoted_string", "\"multiple words\""},
    {"quoted_string", "\"words with something \\\"in quotes\\\"\""},
    {"quoted_string", "\"words\n  including a newline.\""},
    {"quoted_string", "\"a really pathological case \\\\\""}};
  ASSERT_FIND_EXPECTED_TOKENS(expected, lexer);
}


int main(int argc, char ** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
