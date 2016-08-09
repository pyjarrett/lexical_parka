#pragma once

#include "symbol.hpp"

#include <iostream>
#include <sstream>
#include <string>

using std::stringstream;

template<typename T>
std::string as_string(T const & value) {
  std::stringstream ss;
  ss << value;
  return ss.str();
}


namespace context_free {
    
using std::ostream;

ostream & operator<<(ostream & os, Symbol_String const & symbol_string);
ostream & operator<<(ostream & os, vector<Symbol_String> const & symbol_string_vec);

} // namespace context_free
