#pragma once

#include "symbol.hpp"

#include <map>

namespace context_free {

class Grammar {
  std::map<Symbol, Symbol_String_Alternatives> productions;

public:
  Symbol_String_Alternatives & operator[](Symbol const & sym) {
    return productions[sym];
  }
};



} // namespace context_free
