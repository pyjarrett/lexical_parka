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

  bool has_empty_production_direct(Symbol const & sym) const;
  bool has_empty_production(Symbol const & sym) const;
  bool is_empty_production(Symbol_String const & symbol_string) const;
  Symbol_Set empty_producing_symbols() const;
};



} // namespace context_free
