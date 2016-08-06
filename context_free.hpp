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
  static Symbol Non_Terminal(string const & repr);
  static Symbol Terminal(string const & repr);

  /**
   * The "empty" (epsilon) symbol.
   */
  static Symbol Empty();

  string repr() const { return repr_; }
  bool is_terminal() const { return is_terminal_; }

  /**
   * Comparison operator to allow storage of Symbol inside of std::set<>
   */
  bool operator<(Symbol const & other) const;

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
vector<Symbol_String> operator|(Symbol_String const & lhs, Symbol_String const & rhs);
vector<Symbol_String> operator|(vector<Symbol_String> const & lhs, Symbol_String const & rhs);


// Printing functions
using std::ostream;

ostream & operator<<(ostream & os, Symbol const & symbol);
ostream & operator<<(ostream & os, Symbol_String const & symbol_string);
ostream & operator<<(ostream & os, vector<Symbol_String> const & symbol_string_vec);


/**
 * Defines grammatical productions of the type A -> B.
 *
 * Elements in B take the following form:
 * terminals - atomic symbol
 * literals - literal strings
 * non-terminals - symbols which produce other symbols
 * | - "or" conditions, which are alternative productions
 * epsilon - the empty production
 */
class Grammar {

public:


};

} // namespace context_free
