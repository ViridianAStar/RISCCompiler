//
// Created by bowma on 11/7/2025.
//

#include "Parser.h"

// Need way to error check and validate that multi line tokens are correct!

TVEC Parser::compressTokens(const TVEC &tokens) {
    TVEC outputTokens = {};
    
    for (int i = 0; i < tokens.size(); i++) {
        
        Token output_Token = tokens[i];
        
        if (Token const& current_Token = tokens[i]; current_Token.type == TT::Operator) {
            
            if (Token const& next_Token = tokens[i+1]; next_Token.type == TT::Operator) {
                
                const STR value = current_Token.value + next_Token.value;
                output_Token = Token(TT::Operator, value, current_Token.line, current_Token.column, "Complex Operator");
                i++;

                for (int k = i; k < tokens.size(); k++) {
                    
                    Token current = tokens[k];
                    current.column--;
                    
                }
                
            }
            
        }
        outputTokens.push_back(output_Token);
    }
    return outputTokens;
}
