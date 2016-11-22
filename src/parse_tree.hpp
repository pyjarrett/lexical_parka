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
public:
  // using shared_ptr here because a second pointer (in the stack of the
  // parser) is needed when building the Parse_Tree
  using Parse_Tree_Child = std::shared_ptr<Parse_Tree_Node>;
  using Parse_Tree_Children = std::vector<Parse_Tree_Child>;

  Parse_Tree_Node();
  explicit Parse_Tree_Node(Token token);
  Parse_Tree_Node(Token token, std::weak_ptr<Parse_Tree_Node> parent);
  virtual ~Parse_Tree_Node();

  // Should be only using pointers to Parse_Tree_Node.
  Parse_Tree_Node(Parse_Tree_Node const &) = delete;
  Parse_Tree_Node& operator=(Parse_Tree_Node const &) = delete;
  Parse_Tree_Node(Parse_Tree_Node &&) = delete;
  Parse_Tree_Node& operator=(Parse_Tree_Node &&) = delete;

  // TODO: Interface here seems quite a bit clunky.  Almost considering
  // making instance variables public.
  Token const & token() const { return token_; }
  void set_lexeme(string const & lexeme);

  void detach_from_parent();
  void set_children(Parse_Tree_Children & children);

  void print(ostream& os, size_t depth=0);
  string yield() const;

private:
  std::weak_ptr<Parse_Tree_Node> parent_;
  Parse_Tree_Children children_;
  Token token_;

  // Sometimes nodes look the same, but an ID allows for checking during
  // debugging to see if that is really the case.
  static size_t next_id;
  size_t id_;

  void yield_helper(ostream & os, bool is_furthest_left=true) const;
};


class Basic_Parse_Tree_Builder
{
public:
  using value_type = std::shared_ptr<Parse_Tree_Node>;

  template<typename... Args>
  value_type create_node(Args && ...args)
  {
    return std::make_shared<Parse_Tree_Node>(std::forward<Args>(args)...);
  }
};


} // namespace parka
