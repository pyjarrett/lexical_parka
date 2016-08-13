#include "streams.hpp"

namespace context_free {

ostream &
operator<<(
  ostream & os,
  Symbol_String const & symbol_string)
{
  string separator = "";
  for (auto sym : symbol_string) {
    os << separator << sym.repr();
    separator = " ";
  }
  return os;
}

ostream & operator<<(
  ostream & os,
  Symbol_String_Alternatives const & symbol_string_vec)
{
  string separator = "";
  for (auto sym_str : symbol_string_vec) {
    os << separator << sym_str;
    separator = " | ";
  }
  return os;
}

ostream & operator<<(
  ostream & os,
  Symbol_Set const & symbol_set)
{
  string separator = "";
  for (auto const & symbol : symbol_set) {
    std::cout << separator << symbol;
    separator = " ";
  }
  return os;
}

} // namespace context_free
