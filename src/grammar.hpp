#pragma once

#include "symbol.hpp"

#include <map>

namespace context_free {

class Grammar {
  std::map<Symbol, Symbol_String_Alternatives> productions;

  void add_terminals_to_first(std::map<Symbol, Symbol_Set> & first_map) const;

public:
  Symbol_String_Alternatives & operator[](Symbol const & sym) {
    return productions[sym];
  }

  bool is_empty_production(Symbol_String const & symbol_string) const;
  bool has_empty_production(Symbol const & sym) const;
  Symbol_Set empty_producing_symbols() const;

  bool is_terminal(Symbol const & symbol) const;

  std::map<Symbol, Symbol_Set> first() const;
  Symbol_Set first(Symbol const & symbol) const;
};



} // namespace context_free
