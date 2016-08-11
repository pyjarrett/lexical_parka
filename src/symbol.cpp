#include "symbol.hpp"

#include <iostream>

namespace context_free {

Symbol
Symbol::empty() {
  return Symbol("empty");
}


Symbol::operator Symbol_String() const {
  return (Symbol_String) {*this};
}


// In Line grammar creation
Symbol
operator"" _sym(char const * symbol, unsigned long) {
  return Symbol(symbol);
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
