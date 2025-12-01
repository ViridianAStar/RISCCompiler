//
// Created by bowma on 11/5/2025.
//

#include "AST.h"

ANVEC AST::operationsProcessor(ANVEC partialLine) {
    return {}; // TODO later. Potentially invent lambda calculus or something idk.
}


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
                    printf("Did this");
                } else if (node.subType == ST::TypeStatement) {
                    if (i != 0 && i + 3 != line.size() - 1) {
                        auto next = line[i + 1];
                        auto over = line[i + 2];
                        if (const auto& final = line[i + 3]; next.subType == ST::NameStatement && over.subType == ST::AssignmentStatement) {
                            if ( final.subType == ST::DataStatement || final.subType == ST::NameStatement) {
                                next.addChild(node);
                                over.addChild(next);
                                over.addChild(final);
                                output.addChild(over);
                                i += 3; // Simplest case gets reprocessed and translates later.
                            } else if (final.type == ANT::Operation) {
                                // TODO: Process this like a regular operation
                                // for now we can assume operations can only be done one at a time.
                            }
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
                break; // These don't have easy process paths and require incredible branching
            case ANT::Operation:
                if (node.subType == ST::UnaryExpression) {
                    if (const auto& next = line[i + 1]; next.type == ANT::Variable) {
                        if (next.subType == ST::NameStatement || next.subType == ST::DataStatement) {
                            node.addChild(next);
                            output.addChild(node);
                            i += 1; // TODO: Extend these cases
                            // for now assume operations can only be done one at a time.
                        }
                    }
                } else if (node.subType == ST::BinaryExpression) {
                    if (i!=0 || i + 1 != line.size()) {
                        const auto& previous = line[i - 1];
                        if (const auto& next = line[i+1]; (previous.subType == ST::DataStatement || previous.subType == ST::NameStatement) &&  (next.subType == ST::NameStatement || next.subType == ST::DataStatement)) {
                            node.addChild(previous);
                            node.addChild(next);
                            output.addChild(node);
                            i += 1;
                        }
                    }
                    // TODO: Process these. 2 + 2 is easy but what about 3+4+5 or x*7+6?
                    // for now assume operations can only be done one at a time.
                }
                break;

        }
    }
    return output;
}
