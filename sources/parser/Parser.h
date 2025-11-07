//
// Created by bowma on 11/7/2025.
//

#ifndef RISCCOMPILER_PARSER_H
#define RISCCOMPILER_PARSER_H

#include "../tokenizer/Tokenizer.h"
#include <string>
#include <map>

using STR = std::string;

inline std::pmr::map<STR, STR> multiCharOp = {{"==", "equality check"}, {"=<", "less than or equal to"}, {"=>", "greater than or equal to"}};


class Parser {
};


#endif //RISCCOMPILER_PARSER_H