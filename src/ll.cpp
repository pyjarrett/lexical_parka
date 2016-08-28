#include "ll.hpp"

#include "streams.hpp"

namespace context_free {
  /**
   * Creates a predictive parsing table (for a recursive-decent parser with no
   * backtracking) if possible for a LL(1) grammar.
   *
   * \param parsing_table the parsing table to write the result to.  The state
   * of the parsing table is undefined if parsing fails.  Passing by pointer,
   * rather than reference helps make it clearer to clients that they should
   * expect the parsing table to modified.
   *
   * \todo My Rust programming is getting to me as I feel this would be much
   * better implemented using an Optional<>.  In short, I am NOT HAPPY with how
   * this is implemented currently, and will revisit it in the future.  Perhaps
   * exceptions would simplify error handling.  Also, making "productions"
   * public on the grammar would allow this member function to become a free
   * function.  Implementing this function as a "pure" function would also
   * improve it.  I also had to include "steams.hpp" for output of symbols,
   * which isn't required by the rest of this source file, which also indicates
   * a malady here.
   */
  bool
  create_predictive_parsing_table(
      Grammar const & grammar
    , Predictive_Parsing_Table * parsing_table)
  {
    if (parsing_table == nullptr) {
      return false;
    }

    for (auto const & production : grammar.productions()) {
      auto const head = production.first;

      for (auto const & alternative : production.second) {
        // Local macro to simplify error handling and reporting.
#define fail_if_insert_over_existing_mapping(elem, some_production)   \
        if (parsing_table->count(Symbol_Pair(head, elem)) != 0) {     \
          std::cerr << "Ambiguity creating predicting parser table: " \
          << '[' << head << ',' << elem << "] already mapped to "     \
          << (*parsing_table)[Symbol_Pair(head,elem)].first << " -> " \
          << '"' << (*parsing_table)[Symbol_Pair(head,elem)].second << '"' \
          << " but tried to insert " << some_production.first << " -> "    \
          << '"' << some_production.second << '"' << '\n';            \
          return false;                                               \
        }

        auto const current_production = Production(head, alternative);

        // Add A -> alpha for each terminal in a.
        // This was an errata in the Dragon Book
        // using first(alternative) instead of first(head), though this could
        // just be a unclear interpretation of the text.
        auto const first_alt = grammar.first(alternative);
        for (auto const a : first_alt) {
          if (a != Symbol::empty()) {
            fail_if_insert_over_existing_mapping(a, current_production);
            (*parsing_table)[Symbol_Pair(head, a)] = current_production;
          }
        }

        auto const follow_a = grammar.follow(head);
        // Adds A -> alpha for follow and right end marker (if applicable)
        if (first_alt.count(Symbol::empty()) > 0) {
          for (auto const b : follow_a) {
            if (b != Symbol::empty() && b != Symbol::right_end_marker()) {
              fail_if_insert_over_existing_mapping(b, current_production);
              (*parsing_table)[Symbol_Pair(head, b)] = current_production;
            }
          }

          if (follow_a.count(Symbol::right_end_marker()) > 0) {
            fail_if_insert_over_existing_mapping(Symbol::right_end_marker(), current_production);
            (*parsing_table)[Symbol_Pair(head, Symbol::right_end_marker())] = current_production;
          }
        }
#undef fail_if_insert_over_existing_mapping
      }
    }
    return true;
  }
} // namespace context_free
