#pragma once

#include "grammar.hpp"

#include <sstream>
#include <stack>

namespace context_free {

using Predictive_Parsing_Table = std::map<std::pair<Symbol, Symbol>, Production>;

bool create_predictive_parsing_table(
    Grammar const & grammar
  , Predictive_Parsing_Table * parsing_table);

#if 0
template <typename PrintType>
class PrintVisitor
{
public:
  void operator()(PrintType const &) {};
};

template <>
class PrintVisitor<Production>
{
public:
  PrintVisitor
  void operator()(Production const & production)
  {
  }
};

/**
 * Runs the predictive parser algorithm.
 */
template <typename IterableTokenType, typename VisitorFunctor>
void predictive_parse(
    Predictive_Parsing_Table const & ppt
  , Grammar const & grammar
  , IterableTokenType & tokens
  , VisitorFunctor & visitor)
{
  // Set X to the top stack symbol
  auto stack = std::stack<decltype(*tokens.begin())>();

#define error() std::cerr << "Encountered Error\n";
#define advance_ip()

  Symbol X;
  Symbol a;
  
  // Push the start symbol onto the stack followed by the right end marker ($)
  while (X != Symbol::right_end_marker()) {
    if (X == stack.top()) {
      stack.pop();
      advance_ip();
    }
    // if terminal
    else if (grammar.is_terminal(X)) {
      error();
    }
    // if error
    else if (ppt[X, a].is_error()) {
      error();
    }
    // if production
    else if (ppt[X, a].is_production()) {
      visitor(ppt[X, a]);
      stack.push(ppt[X, a].reserve());
      // assert (X1 on top of stack)
    }
    // set X to the top stack symbol.
  }
#undef error()
}

#endif
} // namespace context_free
