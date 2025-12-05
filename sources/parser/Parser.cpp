//
// Created by bowma on 11/7/2025.
//

#include "Parser.h"
#include "../Exceptions/parsing_exception.cpp"

// Need way to error check and validate that multi line tokens are correct!

TVEC Parser::compressTokens(TVEC tokens) {
    TVEC outputTokens = {};

    for (int i = 0; i < tokens.size(); i++) {
        Token output_Token = tokens[i];

        if (Token const &current_Token = tokens[i]; current_Token.type == TT::Operator) {
            if (i == tokens.size() - 1) {
                throw parsing_exception("Lines should end with a semicolon! Line: ", current_Token.line,
                                        current_Token.value);
            }

            if (Token const &next_Token = tokens[i + 1]; next_Token.type == TT::Operator) {
                const STR value = current_Token.value + next_Token.value;

                if (!rebindOperator.contains(value) || !multiCharOp.contains(value)) {
                    throw parsing_exception("Symbol not recognize on line ", 1, value);
                }

                output_Token = Token(TT::Operator, value, current_Token.line, current_Token.column, multiCharOp[value]);
                i++;

                for (int k = i; k < tokens.size(); k++) {
                    tokens[k].decCol();
                }
            }
        }
        outputTokens.push_back(output_Token);
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


