#pragma once

#include "lexer.hpp"

#include <memory>
#include <vector>

namespace parka
{

/**
 * A node in the parse tree (concrete syntax tree).  Contains a token and
 * children, along with some arbitrarily typed additional data (probably note
 * attributes).
 *
 * Dealing with nodes as trees here because it allows nodes to themselves be
 * treated as subtrees.
 */
template<typename Data>
class Parse_Tree_Node
{
  // using shared_ptr here because a second pointer (in the stack of the
  // parser) is needed when building the Parse_Tree
  using Parse_Tree_Children = std::vector<std::shared_ptr<Parse_Tree_Node<Data>>>;

public:
  std::weak_ptr<Parse_Tree_Node> parent;
  Parse_Tree_Children children;
  Token token;
  Data data;

  // Sometimes nodes look the same, but an ID allows for checking during
  // debugging to see if that is really the case.
  static size_t next_id;
  size_t id;

public:
  Parse_Tree_Node();
  virtual ~Parse_Tree_Node();

  void print(std::ostream& os, size_t depth=0);
  std::string yield() const;

private:
  void yield_helper(std::ostream & os) const;

public:
  // Should be only using pointers to Parse_Tree_Node.
  Parse_Tree_Node(Parse_Tree_Node const &) = delete;
  Parse_Tree_Node& operator=(Parse_Tree_Node const &) = delete;
  Parse_Tree_Node(Parse_Tree_Node &&) = delete;
  Parse_Tree_Node& operator=(Parse_Tree_Node &&) = delete;
};


template<typename Data>
size_t Parse_Tree_Node<Data>::next_id = 0;

template<typename Data>
Parse_Tree_Node<Data>::Parse_Tree_Node()
{
  id = ++next_id;
}


template<typename Data>
Parse_Tree_Node<Data>::~Parse_Tree_Node()
{
}


#include <iomanip>
template<typename Data>
void
Parse_Tree_Node<Data>::print(std::ostream & os, size_t depth)
{
  os << std::setw(depth * 2) << std::right << token.symbol << ' ' << token.lexeme << "  ID=" << id << '\n';
  for (auto & child : children) {
    child->print(os, depth+1);
  }
}


#include <sstream>
template<typename Data>
std::string
Parse_Tree_Node<Data>::yield() const
{
  std::stringstream ss;
  yield_helper(ss);
  return ss.str();
}

template<typename Data>
void
Parse_Tree_Node<Data>::yield_helper(std::ostream & os) const
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

class Lisp_Parse_Tree_Builder
{
public:
  using value_type = std::shared_ptr<Parse_Tree_Node<int>>;

  value_type create_node(Token t)
  {
    value_type node = std::make_shared<Parse_Tree_Node<int>>();
    node->token = t;
    return node;
  }
};


} // namespace parka
