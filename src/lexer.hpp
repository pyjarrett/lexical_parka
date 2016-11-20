#pragma once

#include "regex.hpp"
#include "streams.hpp"
#include "string.hpp"
#include "symbol.hpp"

#include <iosfwd>
#include <list>

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


/**
 * A black-box to perform lexical analysis.  Provides configuration which it
 * uses to split up text into (possibly) numerous tokens_.  Lexers each are
 * independent, so create as many as you need.
 *
 * The order in which patterns should reflect their match priority.
 *
 * @section Usage
 * Create a lexer and initialize it with the patterns you need.  Feed it as many
 * different input streams as you like, though you don't need to eat all the
 * tokens_ it produces to feed it more input.
 *
 * @section Error detection and handling.
 * If there are no patterns matching the current input, the lexer will give a
 * lexical exception along with the offending string.  Following this, it will
 * continue lexing, but only after it finds an matched and ignored pattern.
 */
class Lexer {
  string ignore_characters_;
  std::list<std::pair<regex, Symbol>> token_patterns_;
  std::list<Token> tokens_;

public:
  Lexer();

  void register_pattern_for_token(string const & pattern, string const & token_name);
  void register_keyword(string const & keyword);

  void lex(string const & str);
  void lex(istream & input);

  bool has_next_token() const;
  Token next_token();

  bool is_ignored(char ch) const {
    return std::find(ignore_characters_.cbegin(), ignore_characters_.cend(), ch) != ignore_characters_.cend();
  }
};

} // namespace parka
