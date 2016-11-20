#pragma once

#include "lexer.hpp"
#include "streams.hpp"
#include "string.hpp"

#include <memory>
#include <vector>

namespace parka
{

/**
 * A node in the parse tree (concrete syntax tree), containing a token and
 * children resulting from the production with this node as the head.
 *
 * Dealing with nodes as trees here because it allows nodes to themselves be
 * treated as subtrees.
 */
class Parse_Tree_Node
{
  // using shared_ptr here because a second pointer (in the stack of the
  // parser) is needed when building the Parse_Tree
  using Parse_Tree_Children = std::vector<std::shared_ptr<Parse_Tree_Node>>;

public:
  std::weak_ptr<Parse_Tree_Node> parent;
  Parse_Tree_Children children;
  Token token;

  // Sometimes nodes look the same, but an ID allows for checking during
  // debugging to see if that is really the case.
  static size_t next_id;
  size_t id;

public:
  Parse_Tree_Node();
  virtual ~Parse_Tree_Node();

  void print(ostream& os, size_t depth=0);
  string yield() const;

private:
  void yield_helper(ostream & os) const;

public:
  // Should be only using pointers to Parse_Tree_Node.
  Parse_Tree_Node(Parse_Tree_Node const &) = delete;
  Parse_Tree_Node& operator=(Parse_Tree_Node const &) = delete;
  Parse_Tree_Node(Parse_Tree_Node &&) = delete;
  Parse_Tree_Node& operator=(Parse_Tree_Node &&) = delete;
};


class Lisp_Parse_Tree_Builder
{
public:
  using value_type = std::shared_ptr<Parse_Tree_Node>;

  value_type create_node(Token t)
  {
    value_type node = std::make_shared<Parse_Tree_Node>();
    node->token = t;
    return node;
  }
};


} // namespace parka
