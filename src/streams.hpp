#pragma once

#include "string.hpp"
#include "symbol.hpp"

#include <iostream>
#include <sstream>

namespace parka {

// Provide the ability to change out stream types by just changing the string
// type.
using stringstream = std::basic_stringstream<char_type>;
using istream = std::basic_istream<char_type>;
using ostream = std::basic_ostream<char_type>;

template<typename CharT>
struct IO_Streams {};

template<>
struct IO_Streams<std::istream::char_type> {
  // assert input/output char types match
  static std::istream & in;
  static std::ostream & err;
  static std::ostream & out;
};

template<>
struct IO_Streams<std::wistream::char_type> {
  // assert input/output char types match
  static std::wistream & in;
  static std::wostream & err;
  static std::wostream & out;
};

using IO = IO_Streams<char_type>;


template<typename T>
string as_string(T const & value) {
  stringstream ss;
  ss << value;
  return ss.str();
}

ostream & operator<<(ostream & os, Symbol_String const & symbol_string);
ostream & operator<<(ostream & os, vector<Symbol_String> const & symbol_string_vec);
ostream & operator<<(ostream & os, Symbol_Set const & symbol_set);

} // namespace parka
