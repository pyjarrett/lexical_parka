#pragma once

#include "grammar.hpp"
#include "streams.hpp"

#include <stack>

namespace context_free {

using Predictive_Parsing_Table = std::map<std::pair<Symbol, Symbol>, Production>;
using Token = Symbol;


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
  stack.push(Symbol::right_end_marker());
  stack.push(grammar.start_symbol());

#define error(err) std::cerr << "Encountered Error:" #err "\n"; return;

  auto X = stack.top();
  auto next_token_it = tokens.begin();
  
  // Push the start symbol onto the stack followed by the right end marker ($)
  while (X != Symbol::right_end_marker()) {
    auto lookup = std::make_pair(X, *next_token_it);

    // Next input is terminal matching stack top.
    if (X == *next_token_it) {
      visitor(*next_token_it);
      stack.pop();

      // Go to next input.
      ++next_token_it;
    }
    else if (grammar.is_terminal(X)) {
      std::cout << "predictive_parse[error at unmapped terminal]" << X << std::endl;
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
          stack.push(*it);
        }
      }
    }
    X = stack.top();
  }
#undef error
}
} // namespace context_free
