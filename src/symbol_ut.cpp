#include <gtest/gtest.h>

#include "symbol.hpp"
using namespace context_free;

#include <iostream>
#include <sstream>
#include <string>
using std::cout;
using std::string;
using std::stringstream;

template<typename T>
string as_string(T const & value) {
  stringstream ss;
  ss << value;
  return ss.str();
}

vector<int> my_vec;

TEST(First_Test, Examples) {
  Symbol e = Symbol::Non_Terminal("E");
  Symbol e_ = Symbol::Non_Terminal("E'");
  Symbol t = Symbol::Non_Terminal("T");
  Symbol t_ = Symbol::Non_Terminal("T'");
  Symbol f = Symbol::Non_Terminal("F");
  Symbol identifier = "id"_sym;
  Symbol empty = Symbol::Empty();

  ASSERT_EQ("E", as_string(e));
  ASSERT_EQ("E F", as_string(e + f));
  ASSERT_EQ("E | F", as_string(e | f));
  ASSERT_EQ("E F | F E", as_string(e + f | f + e));

  ASSERT_EQ("T E'", as_string(t + e_));
  ASSERT_EQ("+ T E' | empty", as_string("+"_sym + t + e_ | empty));
  ASSERT_EQ("F T'", as_string(f + t_));
  ASSERT_EQ("* F T' | empty", as_string("*"_sym + f + t_ | empty));
  ASSERT_EQ("( E ) | id", as_string("("_sym + e + ")"_sym | "id"_sym));
}

// (4.30)
// FIRST(F) = FIRST(T) = FIRST(E) = { (, id }
// FIRST(E') = { +, e }
// FIRST(T') = { *, e }
// 
// (4.28)
// E  -> T E'
// E' -> + T E' | e
// T  -> F T'
// T' -> * F T' | e
// F  -> ( E ) | id

int main(int argc, char ** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
