#include <gtest/gtest.h>

#include <set>
#include <string>
#include <vector>

using std::string;

struct Grammar_Symbol {
  string content;
  bool terminal;
};

bool operator<(Grammar_Symbol const & left, Grammar_Symbol const & right) {
  return left.content < right.content;
}

using Symbol_Set = std::set<Grammar_Symbol>;
using Symbol_String = std::vector<Grammar_Symbol>;

/**
 * Finds the set of terminals that begin strings derived from a.
 * @param a any string of grammer symbols
 */
Symbol_Set
first(Symbol_String const & a) {
  Symbol_Set output;
  if (a.size() == 1 and a[0].terminal) {
    output.insert(a[0]);
  }
  return output;
}

TEST(First_Test, Terminal_Test) {
  Grammar_Symbol terminal = {"X", true};
  ASSERT_EQ(1, first({terminal}).size());
  ASSERT_TRUE(first({terminal}).count(terminal));
}


int main(int argc, char ** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
