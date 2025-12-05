//
// Created by bowma on 11/7/2025.
//

#include "Parser.h"
//#include "../Exceptions/parsing_exception.cpp"

// Need way to error check and validate that multi line tokens are correct!

TVEC Parser::compressTokens(const TVEC &tokens) {
    TVEC outputTokens = {};

    for (int i = 0; i < tokens.size(); i++) {
        Token current_Token = tokens[i];


        if (current_Token.type == TT::Operator && i + 1 < tokens.size()) {
            const Token& next_Token = tokens[i + 1];



            if (STR combined = current_Token.value + next_Token.value; multiCharOp.contains(combined)) {
                current_Token.value = combined;
                current_Token.meta = multiCharOp[combined];
                current_Token.type = TT::Operator;
                i++;
            }
        }

        outputTokens.push_back(current_Token);
    }

    return outputTokens;
}



ANVEC Parser::parseLine(const TVEC &line) {
    const TVEC workingLine = compressTokens(line);
    ANVEC output = {};
    for (const auto &token: workingLine) {
        switch (token.type) {
            case TT::ControlOperator:
            case TT::Operator:
            case TT::Keyword:
            case TT::Semicolon:
            case TT::Eol:
            case TT::Eof:
                if (rebindOperator.contains(token.value)) {
                    output.emplace_back(STHierarchy[rebindOperator[token.value]], rebindOperator[token.value],
                                             token.line, token.column, token.meta, token.value);
                }
                break;
            case TT::Identifier:
                output.emplace_back(STHierarchy[ST::NameStatement], ST::NameStatement, token.line, token.column,
                                         token.meta, token.value);
                break;
            case TT::Data:
                output.emplace_back(STHierarchy[ST::DataStatement], ST::DataStatement, token.line, token.column,
                                         token.meta, token.value);
                break;
        }
    }
    return output;
}

VEC<ANVEC> Parser::parseFile(const VEC<TVEC>& tokens) {
    VEC<ANVEC> output= {};
    for (const auto &tokenLine: tokens) {
        output.push_back(parseLine(tokenLine));
    }

    return output;
}


