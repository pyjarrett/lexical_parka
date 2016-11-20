#include "parse_tree.hpp"

#include "streams.hpp"
#include "string.hpp"

#include <iomanip>

namespace parka {


size_t Parse_Tree_Node::next_id = 0;

Parse_Tree_Node::Parse_Tree_Node()
{
  id = ++next_id;
}


Parse_Tree_Node::~Parse_Tree_Node()
{
}


void
Parse_Tree_Node::print(ostream & os, size_t depth)
{
  os << std::setw(depth * 2) << std::right << token.symbol << ' ' << token.lexeme << "  ID=" << id << '\n';
  for (auto & child : children) {
    child->print(os, depth+1);
  }
}

string
Parse_Tree_Node::yield() const
{
  stringstream ss;
  yield_helper(ss);
  return ss.str();
}


void
Parse_Tree_Node::yield_helper(ostream & os) const
{
  if (children.size() > 0) {
    for (auto it = children.begin(); it != children.end(); ++it) {
      if (it != children.begin()) {
        os << ' ';
      }
      (*it)->yield_helper(os);
    }
  }
  else {
    os << token.lexeme;
  }
}

} // namespace parka
