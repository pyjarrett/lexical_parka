#include "lexer.hpp"

#include "streams.hpp"
#include "string.hpp"


namespace parka {

Lexer::Lexer()
  : ignore_characters_(" \t\r\n")
{
}

/**
 * Attempts to use a specific pattern to recognize a specific token.  If a regex
 * cannot be created, this will throw a <code>regex_error</code>.
 *
 * If you are using one of the old & broke C++ regex libraries, this lexer will
 * not work at all for you.  Sorry.
 */
void
Lexer::register_pattern_for_token(
  string const & pattern,
  string const & token)
{
  token_patterns_.push_back(std::make_pair(std::regex(pattern), Symbol {token}));
}


/**
 * Shorthand for creating a word pattern with a similarly named symbol.
 */
void
Lexer::register_keyword(string const & keyword)
{
  token_patterns_.push_back(std::make_pair(std::regex("\\b" + keyword + "\\b"), Symbol {keyword}));
}


void
Lexer::lex(string const & str)
{
  stringstream ss(str);
  lex(ss);
}


void
Lexer::lex(istream & input)
{
  // Builds a buffer of all our input.
  // TODO: Only buffer a certain amount at a time (e.g. 16K)
  // TODO: Very inefficient, but will work.
  string buffer;
  while (input) {
    string next_line;
    std::getline(input, next_line);
    buffer.append(next_line);
    buffer.append("\n");
  }

  // Find the first matching pattern at the current input.
  auto current = buffer.cbegin();
  auto eof = buffer.cend();

  // Continue until end of buffer reached.
  while (current != eof) {
    // Finds next whitespace or end of input.
    while (is_ignored(*current) && current != eof) {
      ++current;
    }

    if (current == eof) {
      return;
    }

    auto found_match = false;
    for (auto regex_token_pair : token_patterns_) {
      std::smatch match;

      // Verify starting at current position.
      if (std::regex_search(current, eof, match, regex_token_pair.first)
          && match[0].first == current)
      {
        tokens_.push_back({regex_token_pair.second, match.str()});
        current = match[0].second;
        found_match = true;
        break;
      }
    }

    // No match found, report an error and move to the next character
    // TODO: Report an error.
    if (!found_match) {
      ++current;
    }
  }
}


bool
Lexer::has_next_token() const
{
  return tokens_.size() != 0;
}


Token
Lexer::next_token()
{
  // FIXME: Throw an exception here if no tokens_ are available.
  auto tk = tokens_.front();
  tokens_.pop_front();
  return tk;
}

} // namespace parka
