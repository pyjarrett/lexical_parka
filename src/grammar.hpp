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

  bool has_empty_production(Symbol const & sym) const {
    auto it = productions.find(sym);

    // Assume any production without a mapping is a terminal.
    if (it == productions.end()) {
      return false;
    }

    for (auto & symbol_string : it->second) {
      // FIXME: Horrendous here. Find a better way.
      if (symbol_string.size() == 1 && symbol_string[0] == Symbol::empty()) {
        return true;
      }
    }
    return false;
  }
};



} // namespace context_free
