#pragma once

#include "symbol.hpp"

#include <map>

namespace context_free {

class Grammar {
  Symbol start_symbol_;
  std::map<Symbol, Symbol_String_Alternatives> productions;

  void add_terminals_to_first(std::map<Symbol, Symbol_Set> & first_map) const;
  bool add_production_to_follow(
      Symbol const & head,
      Symbol_String const & body,
      std::map<Symbol, Symbol_Set> & follow_map) const;

public:
  Grammar();

  // Prohibit moving and copying
  Grammar(Grammar const & other) = delete;
  Grammar(Grammar && other) = delete;
  Grammar & operator=(Grammar const & other) = delete;
  Grammar & operator=(Grammar && other) = delete;

  void set_production(
    Symbol const & head,
    Symbol_String_Alternatives const & alternatives);
  Symbol_String_Alternatives operator[](Symbol const & symbol) const;

  Symbol start_symbol() const { return start_symbol_; }

  bool is_empty_body(Symbol_String const & symbol_string) const;
  bool has_empty_production(Symbol const & sym) const;
  Symbol_Set empty_producing_symbols() const;

  bool is_terminal(Symbol const & symbol) const;

  std::map<Symbol, Symbol_Set> first() const;
  Symbol_Set first(Symbol const & symbol) const;
  Symbol_Set first(Symbol_String const & symbol_string) const;

  std::map<Symbol, Symbol_Set> follow() const;
  Symbol_Set follow(Symbol const & symbol) const;
};



} // namespace context_free
