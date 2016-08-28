#include "lexer.hpp"

#include <iostream>
#include <sstream>


namespace context_free {

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
  std::string const & pattern,
  std::string const & token)
{
  token_patterns_.push_back(std::make_pair(std::regex(pattern), token));
}


void
Lexer::lex(std::string const & str)
{
  std::stringstream ss(str);
  lex(ss);
}


void
Lexer::lex(std::istream & input)
{
  // Builds a buffer of all our input.
  // TODO: Only buffer a certain amount at a time (e.g. 16K)
  // TODO: Very inefficient, but will work.
  std::string buffer;
  while (input) {
    std::string next_line;
    std::getline(input, next_line);
    buffer.append(next_line);
    buffer.append("\n");
  }
  std::cout << "Buffer is \"" << buffer << "\"\n";

  // Find the first matching pattern at the current input.
  auto current = buffer.cbegin();
  auto eof = buffer.cend();
  
  // Continue until end of buffer reached.
  while (current != eof) {
    while (is_ignored(*current))
    {
      ++current;
    }

    for (auto regex_token_pair : token_patterns_) {
      std::smatch match;

      // Possible match
      if (std::regex_search(current, eof, match, regex_token_pair.first)) {
        // Verify starting at current position and next is EOF or ignored character. 
        if (match[0].first == current)
        {
          tokens_.push_back({regex_token_pair.second, match.str()});
          current = match[0].second;
          std::cout << "Found match: " << match.str() << '\n';
        }
        else {
          std::cerr << "Possible match ignored:" << '"' << match.str() << '"' << '\n';
        }
      }
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

} // namespace context_free
