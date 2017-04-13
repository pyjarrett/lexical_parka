add_executable(grammar_ut symbol.cpp streams.cpp grammar.cpp grammar_ut.cpp)
target_link_libraries(grammar_ut libgtest libgmock)
add_test(NAME grammar_ut COMMAND grammar_ut --gtest_color=yes)

add_executable(symbol_ut symbol_ut.cpp streams.cpp symbol.cpp)
target_link_libraries(symbol_ut libgtest libgmock)
add_test(NAME symbol_ut COMMAND symbol_ut --gtest_color=yes)

add_executable(ll_ut ll_ut.cpp ll.cpp grammar.cpp lexer.cpp parse_tree.cpp symbol.cpp streams.cpp symbol.cpp)
target_link_libraries(ll_ut libgtest libgmock)
add_test(NAME ll_ut COMMAND ll_ut --gtest_color=yes)

add_executable(lexer_ut lexer_ut.cpp lexer.cpp symbol.cpp streams.cpp symbol.cpp)
target_link_libraries(lexer_ut libgtest libgmock)
add_test(NAME lexer_ut COMMAND lexer_ut --gtest_color=yes)
