#include "grammar.hpp"

#include "streams.hpp"
#include <utility>

namespace parka {
  Grammar::Grammar()
  : start_symbol_(Symbol::empty())
  {
  }

  Symbol_String_Alternatives
  Grammar::operator[](Symbol const & symbol) const
  {
    auto it = productions_.find(symbol);
    if (it != productions_.end()) {
      return (it->second);
    }
    return Symbol_String_Alternatives();
  }

  void
  Grammar::set_alternatives(
    Symbol const & head,
    Symbol_String_Alternatives const & alternatives)
  {
    productions_[head] = alternatives;

    if (start_symbol_ == Symbol::empty()) {
      start_symbol_ = head;
    }
  }

  bool
  Grammar::is_empty_body(Symbol_String const & symbol_string) const
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
    Symbol_Set result = {Symbol::empty()};
    vector<Production> current_possibles;

    // Initially populate the possible list with the grammar's productions.
    for (auto const & production : productions_) {
      for (auto const & body : production.second) {
        current_possibles.push_back({production.first, body});
      }
    }

    vector<Production> next_possibles;
    bool progress_made = true;

    // Each time around this loop should get quicker has direct productions of
    // empty are removed.
    while (progress_made) {
      next_possibles.clear();
      progress_made = false;

      for (auto const & production : current_possibles) {
        // direction production of empty
        if (is_empty_body(production.second)) {
          result.insert(production.first);
          progress_made = true;
        }
        else {
          bool all_body_symbols_produce_empty = true;
          for (auto const & symbol : production.second) {
            if (result.count(symbol) == 0) {
              all_body_symbols_produce_empty = false;
              next_possibles.push_back(production);
              break;
            }
          }
          // all productions produce empty
          if (all_body_symbols_produce_empty) {
            result.insert(production.first);
            progress_made = true;
          }
        }
      }
      std::swap(current_possibles, next_possibles);
    }
    return result;
  }

  bool
  Grammar::is_terminal(Symbol const & symbol) const {
    return productions_.find(symbol) == productions_.end();
  }

  void
  Grammar::add_terminals_to_first(std::map<Symbol, Symbol_Set> & first_map) const
  {
    // Add all terminal symbols once and head of time since there aren't
    // production rules for them.
    for (auto const & production : productions_) {
      for (auto const & alternative : production.second) {
        for (auto const & body_symbol : alternative) {
            if (is_terminal(body_symbol)) {
              // Prevent adding a symbol multiple times.
              if (first_map.find(body_symbol) == first_map.end()) {
                first_map[body_symbol].insert(body_symbol);
              }
            }
        }
      }
    }
  }

  /**
   * Produces the set of FIRST(X).
   */
  std::map<Symbol, Symbol_Set>
  Grammar::first() const
  {
    std::map<Symbol, Symbol_Set> first_map;
    add_terminals_to_first(first_map);

    bool progress_made = true;
    while (progress_made) {
      progress_made = false;

      for (auto const & production : productions_) {
        auto const & head = production.first;

        // X can produce empty, so add it to FIRST
        if (has_empty_production(head)) {
          if (first_map[head].count(Symbol::empty()) == 0) {
            first_map[head].insert(Symbol::empty());
            progress_made = true;
          }
        }

        // Loop through each body for the head symbol.
        for (auto body : production.second) {
          // Add FIRST of each symbol, until finding one which blocks the empty
          // prefix.
          for (auto const body_symbol : body) {
            // Adds FIRST(body_symbol) to FIRST(head) since it can appear as the
            // first non-empty symbol.
            for (auto const & other : first_map[body_symbol]) {
              if (first_map[head].count(other) == 0) {
                first_map[head].insert(other);
                progress_made = true;
              }
            }

            // The next symbol has no empty productions, so further symbols
            // do not affect FIRST(head).
            if (first_map[body_symbol].count(Symbol::empty()) == 0) {
              break;
            }
          }
        }
      }
    }
    return first_map;
  }

  Symbol_Set
  Grammar::first(Symbol const & symbol) const
  {
    return first()[symbol];
  }

  Symbol_Set
  Grammar::first(Symbol_String const & symbol_string) const {
    Symbol_Set result;
    bool all_have_empty_in_first = true;
    for (auto const & symbol : symbol_string) {
      auto const & symbol_first = first(symbol);
      result.insert(symbol_first.begin(), symbol_first.end());

      // Current symbol cannot result in empty, so further symbols cannot affect
      // FIRST for this string
      if (symbol_first.find(Symbol::empty()) == symbol_first.end()) {
        all_have_empty_in_first = false;
        break;
      }
    }

    // Remove empty if it was added and all symbols are not really empty.
    if (all_have_empty_in_first) {
      result.insert(Symbol::empty());
    }
    else {
      result.erase(Symbol::empty());
    }
    return result;
  }

  std::map<Symbol, Symbol_Set>
  Grammar::follow() const
  {
    std::map<Symbol, Symbol_Set> result;

    result[start_symbol()] = {Symbol::right_end_marker()};

    bool progress_made = true;
    while (progress_made) {
      progress_made = false;
      for (auto const & production : productions_) {
        auto const & head = production.first;
        for (auto const & body : production.second) {
          progress_made = progress_made || add_production_to_follow(head, body, result);
        }
      }
    }
    return result;
  }

  Symbol_Set
  Grammar::follow(Symbol const & symbol) const
  {
    // TODO: Add check to ensure symbol is not a terminal.
    return follow()[symbol];
  }

  bool
  Grammar::add_production_to_follow(
      Symbol const & head,
      Symbol_String const & body,
      std::map<Symbol, Symbol_Set> & follow_map) const
  {
    bool progress_made = false;

    auto it = body.begin();
    while (it != body.end()) {
      auto const & current_symbol = *it;
      ++it;

      if (is_terminal(current_symbol)) {
        continue;
      }

      auto const first_right_side = first(Symbol_String(it, body.end()));
      auto const & follow_a = follow_map[head];
      auto & follow_b = follow_map[current_symbol];

      // Adds all of FOLLOW(A) to FOLLOW(B)
      if (first_right_side.count(Symbol::empty()) > 0) {
        auto const size_before_adding_follow_a = follow_b.size();
        follow_b.insert(follow_a.begin(), follow_a.end());
        progress_made = progress_made || (follow_b.size() != size_before_adding_follow_a);
      }

      auto const size_before_adding_first_beta = follow_b.size();
      follow_b.insert(first_right_side.begin(), first_right_side.end());
      follow_b.erase(Symbol::empty());
      progress_made = progress_made || (follow_b.size() != size_before_adding_first_beta);
    }
    return progress_made;
  }
}
