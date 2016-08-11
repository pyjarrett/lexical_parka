#include "grammar.hpp"

#include <utility>

namespace context_free {
  using Production = std::pair<Symbol, Symbol_String>;

  bool
  Grammar::is_empty_production(Symbol_String const & symbol_string) const
  {
      return symbol_string.size() == 1 && symbol_string[0] == Symbol::empty();
  }

  bool
  Grammar::has_empty_production(Symbol const & symbol ) const
  {
    Symbol_Set empty_production_set = empty_producing_symbols();
    return empty_production_set.find(symbol) != empty_production_set.end();
  }

  /**
   * Returns the set of symbols which result in an empty production either
   * directly, or indirectly.
   *
   * I haven't found (or really looked to be honest) to find how to determine
   * this from any articles.
   *
   * A symbol can possibly result in circular dependencies.  So, we need a way
   * to determine if a symbol results in eventually producing itself, and also
   * if it results in producing multiple other symbols as well.  It would seem
   * that limiting the depth of seach would be prudent, as would be caching the
   * value produced.
   *
   * Examining from empty productions and how they virally expand the production
   * of empty symbols is an inverted approach that seems simpler than a direct
   * approach.
   *
   * The latter ("inverted") approach is used here.
   */
  Symbol_Set
  Grammar::empty_producing_symbols() const
  {
    Symbol_Set empty_strings = {Symbol::empty()};
    vector<Production> current_possibles;

    // Initially populate the possible list with the grammar's productions.
    for (auto alternatives : productions) {
      for (auto production : alternatives.second) {
        current_possibles.push_back({alternatives.first, production});
      }
    }

    vector<Production> next_possibles;
    bool progress_made = true;

    // Each time around this loop should get quicker has direct productions of
    // empty are removed.
    //
    // TODO: Could this be faster if a check was added to remove rules that
    // produce all terminals which are not empty?
    while (progress_made) {
      next_possibles.clear();
      progress_made = false;

      for (auto const & production : current_possibles) {
        // Direction production of empty
        if (is_empty_production(production.second)) {
          empty_strings.insert(production.first);
          progress_made = true;
        }
        else {
          bool all_empties = true;
          for (auto const & symbol : production.second) {
            if (empty_strings.count(symbol) == 0) {
              all_empties = false;
              next_possibles.push_back(production);
              break;
            }
          }
          // All productions produce empty
          if (all_empties) {
            empty_strings.insert(production.first);
            progress_made = true;
          }
        }
      }
      std::swap(current_possibles, next_possibles);
    }
    return empty_strings;
  }
}
