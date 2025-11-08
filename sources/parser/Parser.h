//
// Created by bowma on 11/7/2025.
//

#ifndef RISCCOMPILER_PARSER_H
#define RISCCOMPILER_PARSER_H

#include "../tokenizer/Tokenizer.h"
#include "../ast/astnode/ASTNode.h"
#include <string>
#include <map>

using STR = std::string;
using ANVEC = VEC<ASTNode>;
template<typename K, typename V>
using MAP = std::pmr::map<K, V>;

inline MAP<STR, STR> multiCharOp = {{"==", "equality check"}, {"=<", "less than or equal to"}, {">=", "greater than or equal to"}, {"++", "increment"},
    {"--", "decrement"}, {"!=", "not equals"}, {"<=", "back assign"}, {"=>", "forward assign"}};
inline MAP<STR, SubType> rebindOperator = {{"*", SubType::BinaryExpression}, {"+", SubType::BinaryExpression}, {"/", SubType::BinaryExpression},
    {"-", SubType::BinaryExpression}, {"=", SubType::AssignmentStatement}, {"<", SubType::Comparison}, {">", SubType::Comparison},
    {"==", SubType::Comparison}, {"=<", SubType::Comparison}, {"<=", SubType::Comparison}, {"++", SubType::UnaryExpression},
    {"--", SubType::UnaryExpression},{"!", SubType::UnaryExpression}, {"!=", SubType::Comparison}, {"|", SubType::BinaryExpression},
    {"&", SubType::BinaryExpression},{"<=", SubType::AssignmentStatement}, {"=>", SubType::AssignmentStatement}};


class Parser {
public:
    /**
     * Process tokens for multi character operators and more specific types
     *
     * @param tokens unprocessed line of tokens
     * @return processed line of tokens
     */
    static TVEC compressTokens(const TVEC &tokens);

    /**
     * Parse a line of tokens into a vector of AST nodes.
     *
     * @param line input line of tokens
     * @return vector of ast nodes to be added into a tree
     */
    static ANVEC parseLine(TVEC line);
};


#endif //RISCCOMPILER_PARSER_H