cmake_minimum_required(VERSION 4.0)
project(RISCCompiler)

set(CMAKE_CXX_STANDARD 23)

add_executable(RISCCompiler main.cpp
        includes.h
        sources/tokenizer/Tokenizer.cpp
        sources/tokenizer/Tokenizer.h
        sources/ast/AST.cpp
        sources/ast/AST.h
        sources/ast/astnode/ASTNode.cpp
        sources/ast/astnode/ASTNode.h
        sources/parser/Parser.cpp
        sources/parser/Parser.h
        sources/Exceptions/parsing_exception.cpp
        sources/Exceptions/AST_exception.cpp
)
