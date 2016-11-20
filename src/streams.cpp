#include "streams.hpp"

namespace parka {


std::istream & IO_Streams<std::istream::char_type>::in = std::cin;
std::ostream & IO_Streams<std::ostream::char_type>::err = std::cerr;
std::ostream & IO_Streams<std::ostream::char_type>::out = std::cout;

std::wistream & IO_Streams<std::wistream::char_type>::in = std::wcin;
std::wostream & IO_Streams<std::wostream::char_type>::err = std::wcerr;
std::wostream & IO_Streams<std::wostream::char_type>::out = std::wcout;

template <typename IterableType>
ostream &
output_separated_elements(
  ostream & os,
  IterableType const & iterable,
  string const & separator)
{
  string current_separator("");
  for (auto item : iterable) {
    os << current_separator << item;
    current_separator = separator;
  }
  return os;
}

ostream &
operator<<(
  ostream & os,
  Symbol const & symbol)
{
  return os << symbol.repr();
}

ostream &
operator<<(
  ostream & os,
  Symbol_String const & symbol_string)
{
  return output_separated_elements(os, symbol_string, " ");
}

ostream & operator<<(
  ostream & os,
  Symbol_String_Alternatives const & symbol_string_vec)
{
  return output_separated_elements(os, symbol_string_vec, " | ");
}

ostream & operator<<(
  ostream & os,
  Symbol_Set const & symbol_set)
{
  return output_separated_elements(os, symbol_set, " ");
}

} // namespace parka
