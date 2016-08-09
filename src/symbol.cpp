#include "symbol.hpp"

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
  return Symbol::Terminal("empty");
}


Symbol::operator Symbol_String() const {
  return (Symbol_String) {*this};
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
Symbol_String_Alternatives
operator|(Symbol_String const & lhs, Symbol_String const & rhs) {
  Symbol_String_Alternatives result;
  result.push_back(lhs);
  result.push_back(rhs);
  return result;
}

Symbol_String_Alternatives
operator|(Symbol_String_Alternatives const & lhs, Symbol_String const & rhs) {
  Symbol_String_Alternatives result(lhs);
  result.push_back(rhs);
  return result;
}

} // namespace context_free
