#pragma once

#include "symbol.hpp"

#include <map>

namespace context_free {

class Grammar {
  std::map<Symbol, Symbol_String_Alternatives> productions;

  void add_terminals_to_first(std::map<Symbol, Symbol_Set> & first_map) const;

public:
  void set_production(
    Symbol const & head,
    Symbol_String_Alternatives const & alternatives);
  Symbol_String_Alternatives operator[](Symbol const & symbol) const;

  bool is_empty_body(Symbol_String const & symbol_string) const;
  bool has_empty_production(Symbol const & sym) const;
  Symbol_Set empty_producing_symbols() const;

  bool is_terminal(Symbol const & symbol) const;

  std::map<Symbol, Symbol_Set> first() const;
  Symbol_Set first(Symbol const & symbol) const;
  Symbol_Set first(Symbol_String const & symbol_string) const;
};



} // namespace context_free
