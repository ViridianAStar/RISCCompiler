//
// Created by bowma on 11/7/2025.
//

#ifndef RISCCOMPILER_PARSER_H
#define RISCCOMPILER_PARSER_H

#include "../tokenizer/Tokenizer.h"
#include "../ast/astnode/ASTNode.h"
#include <string>
//#include <map>
#include <unordered_map>

using STR = std::string;
using ANT = ASTNodeType;
using ANVEC = VEC<ASTNode>;
using ST = SubType;
template<typename K, typename V>
using MAP = std::pmr::unordered_map<K, V>;

inline MAP<STR, STR> multiCharOp = {{"==", "equality check"}, {"=<", "less than or equal to"}, {">=", "greater than or equal to"}, {"++", "increment"},
    {"--", "decrement"}, {"!=", "not equals"}, {"<=", "less than or equal to"}, {"=>", "greater than or equal to"}, {"|>", "logical right"}, {"<|", "logical left"}};
inline MAP<STR, ST> rebindOperator = {{"*", ST::BinaryExpression}, {"+", ST::BinaryExpression}, {"/", ST::BinaryExpression},
    {"-", ST::BinaryExpression}, {"=", ST::AssignmentStatement}, {"<", ST::Comparison}, {">", ST::Comparison},
    {"==", ST::Comparison}, {"=<", ST::Comparison}, {"<=", ST::Comparison}, {"++", ST::UnaryExpression},
    {"--", ST::UnaryExpression},{"!", ST::UnaryExpression}, {"!=", ST::Comparison}, {"|", ST::BinaryExpression},
    {"&", ST::BinaryExpression},{"<=", ST::Comparison}, {"=>", ST::Comparison}, {"|>", ST::BinaryExpression},
    {"<|", ST::BinaryExpression}, {"int", ST::TypeStatement}, {"if", ST::IfExpression}, {"else", ST::IfExpression},
    {"while", ST::WhileExpression}, {"for", ST::WhileExpression}};

class Parser {
public:
    /**
     * Process tokens for multi character operators and more specific types
     *
     * @param tokens unprocessed line of tokens
     * @return processed line of tokens
     */
    static TVEC compressTokens(TVEC tokens);

    /**
     * Parse a line of tokens into a vector of AST nodes.
     *
     * @param line input line of tokens
     * @return vector of ast nodes to be added into a tree
     */
    static ANVEC parseLine(const TVEC& line);

    /**
     * Parse a "file" worth of tokens into ast nodes
     *
     * @param tokens vector of token vectors
     * @return vector of ast nodes
     */
    static VEC<ANVEC> parseFile(const VEC<TVEC>& tokens);
};


#endif //RISCCOMPILER_PARSER_H