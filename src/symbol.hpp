#pragma once

#include <iostream>
#include <set>
#include <string>
#include <vector>

namespace context_free {

using std::string;
using std::vector;

class Symbol;
using Symbol_String = vector<Symbol>;
using Symbol_Set = std::set<Symbol>;
using Symbol_String_Alternatives = vector<Symbol_String>;

/**
 * A lightweight, immutable symbol type.
 */
class Symbol {
  string repr_;
  bool is_terminal_;

  Symbol(string repr, bool is_terminal)
    : repr_(repr), is_terminal_(is_terminal)
  {
  }

public:
  static Symbol non_terminal(string const & repr);
  static Symbol terminal(string const & repr);

  /**
   * The "empty" (epsilon) symbol.
   */
  static Symbol empty();

  string repr() const { return repr_; }
  bool is_terminal() const { return is_terminal_; }

  // "Less than" for use in std::set and std::map
  bool operator<(Symbol const & other) const {
      return repr_ < other.repr_;
  }

  /**
   * Quick way to allow conversion into `Symbol_String` for Symbol's to
   * simplify the creation of `operator+` and `operator|` methods.
   *
   * \todo refactor if needed for efficiency to eliminate this and use direct
   * Symbol types for | and + operators.
   */
  operator Symbol_String() const;
};


// In line creation of grammer helpers.
Symbol operator"" _sym(char const * symbol, unsigned long);
Symbol_String operator+(Symbol_String const & lhs, Symbol_String const & rhs);
Symbol_String_Alternatives operator|(Symbol_String const & lhs, Symbol_String const & rhs);
vector<Symbol_String> operator|(vector<Symbol_String> const & lhs, Symbol_String const & rhs);


} // namespace context_free
