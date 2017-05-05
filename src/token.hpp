#pragma once

#include "string.hpp"
#include "symbol.hpp"

namespace parka {

/**
* The atomic unit created by the lexer.
*/
struct Token {
  Symbol symbol;

  /// The contents of the matched pattern.
  string lexeme;

  explicit Token(Symbol const & symbol) : symbol(symbol), lexeme(symbol.repr()) {}
  Token(Symbol const & symbol, string const & lexeme) : symbol(symbol), lexeme(lexeme) {}

  // Per "Modern C++" item 17.
  // Having "null" tokens isn't the worst thing here,
  // and it's perfectly OK to use default copy and move.
  Token() = default;
  ~Token() = default;
  Token(Token const &) = default;
  Token& operator=(Token const &) = default;
  Token(Token&&) = default;
  Token& operator=(Token&&) = default;
};

} // namespace parka