#pragma once

#include <iosfwd>
#include <list>
#include <regex>
#include <string>

namespace parka {

/**
 * The atomic unit created by the lexer.
 */
struct Token {
  std::string symbol_name;

  /// The contents of the matched pattern.
  std::string lexeme;
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
  std::string ignore_characters_;
  std::list<std::pair<std::regex, std::string>> token_patterns_;
  std::list<Token> tokens_;

public:
  Lexer();

  void register_pattern_for_token(std::string const & pattern, std::string const & token_name);
  void register_keyword(std::string const & keyword);

  void lex(std::string const & str);
  void lex(std::istream & input);

  bool has_next_token() const;
  Token next_token();

  bool is_ignored(char ch) const {
    return std::find(ignore_characters_.cbegin(), ignore_characters_.cend(), ch) != ignore_characters_.cend();
  }
};

} // namespace parka
