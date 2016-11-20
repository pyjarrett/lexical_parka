#pragma once

#include "grammar.hpp"
#include "lexer.hpp"
#include "streams.hpp"

#include <stack>

namespace parka {

using Predictive_Parsing_Table = std::map<std::pair<Symbol, Symbol>, Production>;


bool create_predictive_parsing_table(
    Grammar const & grammar
  , Predictive_Parsing_Table * parsing_table);


/**
 * A visitor to just print the production and symbols as they are processed.
 */
class Predictive_Parse_Print_Visitor
{
  std::ostream & stream_;

public:
  Predictive_Parse_Print_Visitor(std::ostream & os) : stream_(os) {}
  void operator()(Symbol const & terminal)
  {
    stream_ << "Matched: " << terminal << '\n';
  }

  void operator()(Production const & production)
  {
    stream_ << production.first << " -> " << production.second << '\n';
  }
};


/**
 * Runs a simple predictive parser algorithm calling a visitor for every
 * terminal and production found.  No error handling is done.
 */
template <typename IterableTokenType, typename VisitorFunctor>
void
predictive_parse(
    Predictive_Parsing_Table const & ppt
  , Grammar const & grammar
  , IterableTokenType & tokens
  , VisitorFunctor & visitor)
{
  // Prepares starting stack as our program followed by "end of input".
  auto stack = std::stack<typename IterableTokenType::value_type>();
  stack.push(Token(Symbol::right_end_marker()));
  stack.push(Token(grammar.start_symbol()));

#define error(err) std::cerr << "Encountered Error:" #err "\n"; return;

  auto X = stack.top();
  auto next_token_it = tokens.begin();

  // Push the start symbol onto the stack followed by the right end marker ($)
  while (X.symbol != Symbol::right_end_marker()) {
    auto lookup = std::make_pair(X.symbol, next_token_it->symbol);

    // Next input is terminal matching stack top.
    if (X.symbol == next_token_it->symbol) {
      X.lexeme = next_token_it->lexeme;
      visitor(next_token_it->symbol);
      stack.pop();

      // Go to next input.
      ++next_token_it;
    }
    else if (grammar.is_terminal(X.symbol)) {
      std::cout << "predictive_parse[error at unmapped terminal]" << X.symbol << std::endl;
      error("Terminal!");
    }
    // if symbol not found, error
    else if (ppt.count(lookup) == 0) {
      error("No production found");
    }
    // if production
    else if (ppt.count(lookup) > 0) {
      visitor(ppt.at(lookup));
      stack.pop();

      // Push Yk, Y(k-1), Y(k-2), ... Y1
      for (auto it = ppt.at(lookup).second.rbegin(); it != ppt.at(lookup).second.rend(); ++it) {
        if (*it != Symbol::empty()) {
          stack.push(Token {*it, ""});
        }
      }
    }
    X = stack.top();
  }
#undef error
}


template <
    typename Iterable_Token_Type
  , typename Parse_Tree_Builder>
auto
predictive_parse_into_parse_tree(
    Predictive_Parsing_Table const & ppt
  , Grammar const & grammar
  , Iterable_Token_Type & tokens
  , Parse_Tree_Builder & builder)
-> typename Parse_Tree_Builder::value_type
{
  // Prepares starting stack as our program followed by "end of input".
  auto stack = std::stack<typename Parse_Tree_Builder::value_type>();
  auto parse_tree_root = builder.create_node(Token(grammar.start_symbol()));

  stack.push(builder.create_node(Token(Symbol::right_end_marker())));
  stack.push(parse_tree_root);

#define error(err) std::cerr << "Encountered Error:" #err "\n"; return nullptr;

  auto X = stack.top();
  auto next_token_it = tokens.begin();

  // Push the start symbol onto the stack followed by the right end marker ($)
  while (X->token.symbol != Symbol::right_end_marker()) {
    auto lookup = std::make_pair(X->token.symbol, next_token_it->symbol);

    // Next input is terminal matching stack top.
    if (X->token.symbol == next_token_it->symbol) {
      X->token.lexeme = next_token_it->lexeme;
      stack.pop();

      // Go to next input.
      ++next_token_it;
    }
    else if (grammar.is_terminal(X->token.symbol)) {
      std::cerr << "predictive_parse[error at unmapped terminal]" << X->token.symbol << std::endl;
      error("Terminal!");
    }
    // if symbol not found, error
    else if (ppt.count(lookup) == 0) {
      error("No production found");
    }
    // if production
    else if (ppt.count(lookup) > 0) {
      stack.pop();

      // Push Yk, Y(k-1), Y(k-2), ... Y1
      auto body = ppt.at(lookup).second;
      for (auto it = body.rbegin(); it != body.rend(); ++it) {
        if (*it != Symbol::empty()) {
          auto node = builder.create_node(Token(*it));

          // Insert into children at index 0 to undo reverse effects of pushing
          // symbols in inverse order of the production.
          X->children.insert(begin(X->children), node);
          stack.push(node);
        }
      }
    }
    X = stack.top();
  }
  return parse_tree_root;
#undef error
}
} // namespace parka
