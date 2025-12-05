//
// Created by bowma on 11/5/2025.
//

#include "Tokenizer.h"
#include <sstream>
#include <iterator>

// private:

bool Tokenizer::validID(const STR &ID) {
    if (const std::regex pattern("^[A-Za-z][A-Za-z0-9_]*$"); std::regex_match(ID.c_str(), pattern)) {
        return true;
    }
    return false;
}

// maybe thread this later

TVEC Tokenizer::lexLine(const STR& line, const int lnum) {
    TVEC toks = {};
    int col = 0;

    std::istringstream splitable(line);
    VEC<STR> words;
    std::for_each(std::istream_iterator<STR>(splitable),
              std::istream_iterator<STR>(),
              [&](const STR& s) { words.push_back(s); });

    static const std::regex delims(R"(([^+\-/*<>=&~;\n]+)|([+\-/*<>=&~;]))");
    VEC<STR> refined;
    for (const auto& w : words) {
        for (std::sregex_iterator it(w.begin(), w.end(), delims), end; it != end; ++it)
            refined.push_back(it->str());
    }
    words = std::move(refined);

    while (col < words.size()) {
        if (const STR& word = words.at(col); keywords.contains(word)) {
            auto tok = Token(TT::Keyword, word, lnum, col);
            tok.meta = keywords[word];
            toks.push_back(tok);
        } else if (operators.contains(word)) {
            if (word == "(" || word == ")") {
                auto tok = Token(TT::ControlOperator, word, lnum, col);
                tok.meta = operators[word];
                toks.push_back(tok);
            } else {
                auto tok = Token(TT::Operator, word, lnum, col);
                tok.meta = operators[word];
                toks.push_back(tok);
            }
        } else if (const std::regex pattern("^[0-9_]$"); std::regex_match(word, pattern)) {
            auto tok = Token(TT::Data, word, lnum, col);
            tok.meta = "Some Data";
            toks.push_back(tok);
        } else if (word == ";") {
            auto tok = Token(TT::Semicolon, word, lnum, col);
            tok.meta = "Semicolon";
            toks.push_back(tok);
        } else {
            if (validID(word)) {
                auto tok = Token(TT::Identifier, word, lnum, col);
                tok.meta = "Some Identifier";
                toks.push_back(tok);
            }
        }

        col++;
    }
    if (toks.at(toks.size() - 1).value == ";") {
        toks.at(toks.size() - 1).type = TT::Eol;
        toks.at(toks.size() - 1).meta = "End of line";
    }
    
    return toks;
}

// public:

VEC<TVEC> Tokenizer::tokenize(const STR &file) {
    VEC<TVEC> tokenizedLines = {};
    VEC<STR> lines;
    std::stringstream fileStream(file);
    STR ulline;
    while (std::getline(fileStream, ulline)) {
        lines.push_back(ulline);
    }

    int lnum = 0;
    for (const auto& line : lines) {
        tokenizedLines.push_back(lexLine(line, lnum));
        lnum++;
    }

    TVEC lastLine = tokenizedLines.at(tokenizedLines.size() - 1);
    if (lastLine.at(lastLine.size() - 1).type == TT::Eol) {
        lastLine.at(lastLine.size() - 1).type = TT::Eof;
        lastLine.at(lastLine.size() - 1).meta = "End of file";
    }

    return tokenizedLines;

}
