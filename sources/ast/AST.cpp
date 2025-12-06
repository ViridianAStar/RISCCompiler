#include "./AST.h"

#include <functional>

ASTNode AST::compressBunchNode(const ANVEC &line, int &i) {
    if (i >= line.size() ||
        line[i].type != ANT::Control ||
        line[i].subType != ST::BunchStart)
        throw AST_exception("Expected grouping start");

    ANVEC compress;
    int depth = 1;
    const int startIndex = i + 1;

    for (int j = startIndex; j < line.size(); j++) {
        const auto &working = line[j];

        if (working.type == ANT::Control) {
            if (working.subType == ST::BunchStart) {
                depth++;
            } else if (working.subType == ST::BunchEnd) {
                depth--;
                if (depth == 0) {
                    i = j;
                    break;
                }
            }
        }

        if (depth > 0) {
            compress.push_back(working);
        }
    }

    if (depth != 0)
        throw AST_exception("Unmatched parentheses");

    ASTNode branchedNode = compressNodes(compress);

    branchedNode.line = line[i].line;
    branchedNode.column = line[i].column;
    branchedNode.type = ASTNodeType::Branch;

    return branchedNode;
}

ASTNode AST::compressNodes(ANVEC &line) {
    auto output = ASTNode(
        ANT::Line,
        ST::Collection,
        line[0].line,
        -1,
        "Line"
        );

    return output;
}

