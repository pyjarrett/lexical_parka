# Lexical Parka #

## Goals ##

My required courses in college always collided with the compiler course, and
OMSCS doesn't offer a compiler course yet, so to learn more about lexers,
parsers, and other parts of compilers I'm writing one from the ground up.  I've
had a good time playing around in the LLVM/Clang source code as well as in the
Rust compiler code, but I've learned a lot by also working on one of my own.

_Why not just use Lex/Yacc/Bison/ANTLR/Boost Spirit?_ This is a learning
experience, everything from writing FIRST(X) to doing shift-reduce, and
hopefully emitting intermediate code at some point.

## Tools ##

I'm allowing myself:

- C++14 to play around with the new features
- [GoogleTest](https://github.com/google/googletest)
- GoogleMock
- [Bazel](https://www.bazel.io)

