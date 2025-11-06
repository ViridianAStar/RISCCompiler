//
// Created by bowma on 11/5/2025.
//

#ifndef RISCCOMPILER_TOKENIZER_H
#define RISCCOMPILER_TOKENIZER_H

#include <string>
#include <utility>
#include <vector>
#include <regex>
#include <algorithm>
#include <map>

struct Token;
template<typename T>
//standard vector shorthand
using VEC = std::vector<T>;
//standard string shorthand
using STR = std::string;
//shorthand for vectors containing tokens.
using TVEC = VEC<Token>;

/**
 * Enum containing valid token information
 */
enum class TokenType {
    Identifier,
    Keyword,
    Operator,
    Data,
    Semicolon,
    Eol,
    Eof
};

#define TT TokenType

//inline STR keywords[] = {"int", "char", "if"};
inline std::pmr::map<STR, STR> keywords = {{"int", "integer"}, {"char", "character"}, {"if", "control flow"}};
//inline STR operators[] = {"*", "+", "/", "-", "="};
inline std::pmr::map<STR, STR> operators = {{"*", "binary expression multiplication"}, {"+", "binary expression addition"},
    {"/", "binary expression division"}, {"-", "binary expression subtraction"}, {"=", "assignment"}};


/**
 * Struct for building a token
 */
struct Token {
    TokenType type;
    STR value;
    int line;
    int column;
    STR meta;

    Token(const TokenType type, STR value, const int line, const int column, STR meta = "No MetaData")
        : type(type), value(std::move(value)), line(line), column(column), meta(std::move(meta)) {}
};

class Tokenizer {
//public:
    /**
     * Validate that an identifier exists.
     *
     * @param ID Identifier to validate using regex/dfa
     * @return boolean value indicating valid identifier
     */
    static bool validID(const STR &ID);

    /**
     * Lex/Tokenize a line of code
     *
     * @param line contents of line we are working on
     * @param lnum line number we are working on
     * @return token vector
     */
    static TVEC lexLine(const STR &line, int lnum);

    public:
    /**
     * Lex/Tokenize an entire file
     *
     * @param file large string representative of a whole file of code
     * @return vector of token vectors representative of individually lexed lines
     */
    static VEC<TVEC> tokenize(const STR &file);
};


#endif //RISCCOMPILER_TOKENIZER_H
