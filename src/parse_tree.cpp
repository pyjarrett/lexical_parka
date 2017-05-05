#include "parse_tree.hpp"

#include "streams.hpp"
#include "string.hpp"

#include <iomanip>

namespace parka {


size_t Parse_Tree_Node::next_id = 0;

Parse_Tree_Node::Parse_Tree_Node()
{
  id_ = ++next_id;
}

Parse_Tree_Node::Parse_Tree_Node(Token token)
  : token_(token)
{
  id_ = ++next_id;
}

Parse_Tree_Node::Parse_Tree_Node(Token token, std::weak_ptr<Parse_Tree_Node> parent)
  : parent_(parent)
  , token_(token)
{
  id_ = ++next_id;
}


Parse_Tree_Node::~Parse_Tree_Node()
{
}


/**
 * The lexeme may not be known when the node is create (e.g. in LL predictive
 * parse).  Provides a way to set it.
 */
void
Parse_Tree_Node::set_lexeme(string const & lexeme)
{
  token_.lexeme = lexeme;
}


void
Parse_Tree_Node::detach_from_parent()
{
  if (!parent_.expired()) {
    auto parent = parent_.lock();
    Parse_Tree_Children::iterator loc_of_x;
    for (loc_of_x = parent->children_.begin(); loc_of_x != parent->children_.end(); ++loc_of_x) {
      if (loc_of_x->get() == this) {
        break;
      }
    }
    if (loc_of_x != parent->children_.end()) {
      parent->children_.erase(loc_of_x);
    }
  }
}


void
Parse_Tree_Node::set_children(Parse_Tree_Children & children)
{
  children_.assign(begin(children), end(children));
}


void
Parse_Tree_Node::print(ostream & os, size_t depth)
{
  os << std::setw(depth * 2) << std::right << token_.symbol << ' ' << token_.lexeme << "  ID=" << id_ << '\n';
  for (auto & child : children_) {
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
Parse_Tree_Node::yield_helper(ostream & os, bool is_furthest_left) const
{
  if (children_.size() > 0) {
    for (auto it = children_.begin(); it != children_.end(); ++it) {
      (*it)->yield_helper(os, (it == children_.begin()) && is_furthest_left);
    }
  }
  else if (token_.symbol != Symbol::empty()) {
    if (!is_furthest_left) {
      os << ' ';
    }
    os << token_.lexeme;
  }
}

} // namespace parka
