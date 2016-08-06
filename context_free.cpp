#include "context_free.hpp"

#include <iostream>

namespace context_free {

Symbol
Symbol::Non_Terminal(string const & repr) {
  return Symbol(repr, false);
}

Symbol
Symbol::Terminal(string const & repr) {
  return Symbol(repr, true);
}

Symbol
Symbol::Empty() {
  return Symbol("empty", true);
}


Symbol::operator Symbol_String() const {
  Symbol_String sym_str;
  sym_str.push_back(*this);
  return sym_str;
}


// In Line grammar creation
Symbol
operator"" _sym(char const * symbol, unsigned long) {
  return Symbol::Terminal(symbol);
}

Symbol_String
operator+(Symbol_String const & lhs, Symbol_String const & rhs) {
  Symbol_String result(lhs);
  result.insert(result.end(), rhs.begin(), rhs.end());
  return result;
}


// Alternatives
vector<Symbol_String>
operator|(Symbol_String const & lhs, Symbol_String const & rhs) {
  vector<Symbol_String> result;
  result.push_back(lhs);
  result.push_back(rhs);
  return result;
}

vector<Symbol_String>
operator|(vector<Symbol_String> const & lhs, Symbol_String const & rhs) {
  vector<Symbol_String> result(lhs);
  result.push_back(rhs);
  return result;
}


// Printing functions
ostream &
operator<<(ostream & os, Symbol const & symbol) {
  os << symbol.repr();
  return os;
}

ostream &
operator<<(ostream & os, Symbol_String const & symbol_string) {
  string separator = "";
  for (auto sym : symbol_string) {
    os << separator << sym;
    separator = " ";
  }
  return os;
}

ostream & operator<<(ostream & os, vector<Symbol_String> const & symbol_string_vec) {
  string separator = "";
  for (auto sym_str : symbol_string_vec) {
    os << separator << sym_str;
    separator = " | ";
  }
  return os;
}

} // namespace context_free
