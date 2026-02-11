//
// Created by bowma on 11/5/2025.
//

//node generation

#ifndef RISCCOMPILER_AST_H
#define RISCCOMPILER_AST_H
#include <cassert>
#include "astnode/ASTNode.h"
#include "../Exceptions/AST_exception.cpp"

using STR = std::string;
template<typename T>
using VEC = std::vector<T>;
using ANT = ASTNodeType;
using ST = SubType;
template<typename K, typename V>
using MAP = std::pmr::unordered_map<K, V>;
using ANVEC = VEC<ASTNode>;

class AST {
public:
    ASTNode startNode = ASTNode(ASTNodeType::File, ST::Collection, -1, -1, "Program");

    static ASTNode compressNodes(ANVEC &line);

    static ASTNode compressBunchNode(const ANVEC &line, int &i);

};

#endif //RISCCOMPILER_AST_H