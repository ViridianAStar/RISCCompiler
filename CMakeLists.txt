cmake_minimum_required(VERSION 3.15)
project(RISCCompiler)

set(CMAKE_CXX_STANDARD 23)

find_package(elfio)


add_executable(RISCCompiler
        main.cpp
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
        sources/ast/translators/RiscV/RiscV.cpp
)


target_link_libraries(RISCCompiler elfio::elfio)
