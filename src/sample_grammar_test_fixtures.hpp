#pragma once

#include "grammar.hpp"
#include "streams.hpp"
#include "symbol.hpp"

#include <gtest/gtest.h>

/**
 * Simple test fixture for a basic grammar with adds and multiply, with multiply
 * taking precedence.
 */
class Add_Multiply_Grammar_Test : public ::testing::Test {
protected:
  context_free::Grammar grammar;

  virtual void SetUp() {
    grammar.set_alternatives("E"_sym, {"E"_sym + "+"_sym + "T"_sym | "T"_sym});
    grammar.set_alternatives("T"_sym, {"T"_sym + "*"_sym + "F"_sym | "F"_sym});
    grammar.set_alternatives("F"_sym, {"("_sym + "E"_sym + ")"_sym | "id"_sym});
  }

  virtual void TearDown() {}
};


/**
 * Add & multiple grammar which is LL(1) and has no left recursion.
 */
class Non_Left_Recursive_Add_Multiply_Grammar_Test : public ::testing::Test {
protected:
  context_free::Grammar grammar;

  virtual void SetUp() {
    grammar.set_alternatives("E"_sym, {"T"_sym + "E'"_sym});
    grammar.set_alternatives("E'"_sym, {"+"_sym + "T"_sym + "E'"_sym | Symbol::empty()});
    grammar.set_alternatives("T"_sym, {"F"_sym + "T'"_sym});
    grammar.set_alternatives("T'"_sym, {"*"_sym + "F"_sym + "T'"_sym | Symbol::empty()});
    grammar.set_alternatives("F"_sym, {"("_sym + "E"_sym + ")"_sym | "id"_sym});
  }

  virtual void TearDown() {}
};
