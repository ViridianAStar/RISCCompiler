//
// Created by bowma on 11/5/2025.
//

#include "AST.h"

ASTNode AST::compressNodes(const ANVEC &line) {
    auto output = ASTNode(ANT::Line, ST::Collection, line[0].line, -1, std::to_string(line[0].line));
    for (int i = 0; i < line.size(); i++) {
        auto node = line[i];
        switch (node.type) {
            case ASTNodeType::File:
                throw AST_exception("Only one file node allowed at this time. line:", node.line, "!");
                break;
            case ASTNodeType::Line:
                throw AST_exception("Lines cannot contain lines! line ", node.line, "!");
                break;
            case ASTNodeType::Variable:
                // TODO: Extend these cases. int i = 0 works but what about int i = func(0)?
                if (node.subType == ST::DataStatement || node.subType == ST::NameStatement) {
                    break;
                } else if (node.subType == ST::TypeStatement) {
                    if (i != 0 && i + 3 != line.size() - 1) {
                        auto next = line[i + 1];
                        auto over = line[i + 2];
                        if (const auto& final = line[i + 3]; next.subType == ST::NameStatement && over.subType == ST::AssignmentStatement && final.
                                                             subType == ST::DataStatement) {
                            next.addChild(node);
                            over.addChild(next);
                            over.addChild(final);
                            i += 3;
                        } else {
                            throw AST_exception("Incomplete statements on line: ", node.line, "!");
                        }
                        break;
                    } else {
                        throw AST_exception("Incomplete statements on line: ", node.line, "!");
                    }
                }
                break;
            case ANT::Definition:
            case ANT::Control:
                break;
            case ANT::Operation:
                if (node.subType == ST::UnaryExpression) {
                    if (const auto& next = line[i + 1]; next.type == ANT::Variable) {
                        if (next.subType == ST::NameStatement || next.subType == ST::DataStatement) {
                            node.addChild(next); // TODO: Extend these cases
                        }
                    }
                } else if (node.subType == ST::BinaryExpression) {
                    break; // TODO: Process these. 2 + 2 is easy but what about 3+4+5 or x*7+6?
                }
                break;

        }
    }
    return output;
}
