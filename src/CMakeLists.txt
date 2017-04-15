unit_test(NAME grammar_ut SOURCES symbol.cpp streams.cpp grammar.cpp)
unit_test(NAME symbol_ut SOURCES symbol_ut.cpp streams.cpp symbol.cpp)
unit_test(NAME ll_ut SOURCES ll.cpp grammar.cpp lexer.cpp parse_tree.cpp symbol.cpp streams.cpp symbol.cpp)
unit_test(NAME lexer_ut SOURCES lexer.cpp symbol.cpp streams.cpp symbol.cpp)
