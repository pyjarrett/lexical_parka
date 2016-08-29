#pragma once

#include <iostream>
#include <set>
#include <string>
#include <vector>

namespace parka {

using std::string;
using std::vector;

class Symbol;
using Symbol_String = vector<Symbol>;
using Symbol_Set = std::set<Symbol>;
using Symbol_String_Alternatives = vector<Symbol_String>;
using Symbol_Pair = std::pair<Symbol, Symbol>;

/**
 * A lightweight, immutable symbol type.
 */
class Symbol {
  string repr_;

public:
  explicit Symbol(string repr="empty")
    : repr_(repr)
  {
  }

  /**
   * The "empty" (epsilon) symbol.
   */
  static Symbol empty();

  /**
   * Typically denoted "$"
   */
  static Symbol right_end_marker();

  string repr() const { return repr_; }

  // "Less than" for use in std::set and std::map
  bool operator<(Symbol const & other) const {
      return repr_ < other.repr_;
  }

  bool operator==(Symbol const & other) const {
    return repr_ == other.repr_;
  }

  bool operator!=(Symbol const & other) const {
    return !((*this) == other);
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
Symbol_String_Alternatives operator|(Symbol_String_Alternatives const & lhs, Symbol_String const & rhs)
  ;


} // namespace parka
