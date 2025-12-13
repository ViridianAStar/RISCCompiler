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

class LinePratt {
public:
    explicit LinePratt(const ANVEC& nodes)
        : nodes(nodes) {}

    ASTNode parseExpression(const int rbp = 0) {
        ASTNode left = nud(advance());

        while (rbp < lbp(peek())) {
            ASTNode op = advance();
            left = led(op, left);
        }

        return left;
    }

private:
    const ANVEC& nodes;
    size_t pos = 0;

    [[nodiscard]] const ASTNode& peek() const {
        return nodes[pos];
    }

    static int precedence(const ASTNode &node) {
        if (node.subType == ST::AssignmentStatement) return 5;
        if (node.subType == ST::Comparison)          return 10;

        if (node.value == "+" || node.value == "-") return 20;
        if (node.value == "*" || node.value == "/") return 30;

        return 0;
    }

    ASTNode advance() {
        return nodes[pos++];
    }

    static int lbp(const ASTNode& n) {
        return precedence(n);
    }

    ASTNode nud(ASTNode n) {

        if (n.subType == ST::NameStatement ||
            n.subType == ST::DataStatement) {
            return n;
            }


        if (n.subType == ST::UnaryExpression) {
            ASTNode node(
                ANT::Operation,
                ST::UnaryExpression,
                n.line,
                n.column,
                n.nodeName
            );
            node.addChild(parseExpression(40));
            return node;
        }

        throw AST_exception("Invalid Expression Start");
    }

    ASTNode led(const ASTNode& op, const ASTNode& left) {

        if (op.subType == ST::AssignmentStatement) {
            ASTNode node(
                ANT::Operation,
                ST::AssignmentStatement,
                op.line,
                op.column,
                op.nodeName
            );
            node.addChild(left);
            node.addChild(parseExpression(precedence(op) - 1));
            return node;
        }

        ASTNode node(
            ANT::Operation,
            op.subType,
            op.line,
            op.column,
            op.nodeName
        );
        node.addChild(left);
        node.addChild(parseExpression(precedence(op)));
        return node;
    }
};



#endif //RISCCOMPILER_AST_H