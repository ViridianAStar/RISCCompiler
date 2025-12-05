//
// Created by bowma on 11/5/2025.
//

#include "AST.h"

ANVEC AST::operationsProcessor(const ANVEC &partialLine) {
    return {}; // TODO later. Potentially invent lambda calculus or something idk.
}

ASTNode AST::compressNodes(ANVEC &line) {
    auto output = ASTNode(
        ANT::Line,
        ST::Collection,
        line[0].line,
        -1,
        std::to_string(line[0].line)
    );

    for (int i = 0; i < line.size(); i++) {
        switch (auto &node = line[i]; node.type) {
            case ASTNodeType::File:
                throw AST_exception("Only one file node allowed at this time. line:", node.line, "!");

            case ASTNodeType::Line:
                throw AST_exception("Lines cannot contain lines! line ", node.line, "!");

            case ASTNodeType::Variable:
                // TODO: Extend these cases. int i = 0 works but what about int i = func(0)?
                if (node.subType == ST::DataStatement || node.subType == ST::NameStatement) {
                    printf("Did this");
                } else if (node.subType == ST::TypeStatement) {

                    if (i + 3 >= line.size()) throw AST_exception("Incomplete statements on line: ", node.line, "!");

                    auto &next = line[i + 1];
                    auto &over = line[i + 2];
                    auto &final = line[i + 3];

                    if (next.subType == ST::NameStatement &&
                        over.subType == ST::AssignmentStatement) {
                        if (final.subType == ST::DataStatement ||
                            final.subType == ST::NameStatement) {
                            next.addChild(node);
                            over.addChild(next);
                            over.addChild(final);
                            output.addChild(over);
                            i += 3;
                        } else if (final.type == ANT::Control) {
                            // TODO: Operation handling
                        }
                    } else throw AST_exception("Incomplete statements on line: ", node.line, "!");
                }
                break;

            case ANT::Definition:
            case ANT::Control:
                // These don't have easy process paths and require incredible branching
                break;

            case ANT::Operation:

                if (node.subType == ST::UnaryExpression) {
                    if (i + 1 < line.size()) {
                        if (auto &next = line[i + 1]; next.type == ANT::Variable &&
                                                      (next.subType == ST::NameStatement ||
                                                       next.subType == ST::DataStatement)) {
                            node.addChild(next);
                            output.addChild(node);
                            i += 1; // TODO: Extend cases
                        }
                    }
                } else if (node.subType == ST::BinaryExpression) {
                    if (i > 0 && i + 1 < line.size()) {
                        auto &previous = line[i - 1];

                        if (auto &next = line[i + 1]; (previous.subType == ST::DataStatement || previous.subType == ST::NameStatement) &&
                                                                    (next.subType == ST::NameStatement || next.subType == ST::DataStatement)) {
                            node.addChild(previous);
                            node.addChild(next);
                            output.addChild(node);
                            i += 1;
                        }
                    }

                    // TODO: Multi-operator expressions (3+4+5, x*7+6, etc.)
                }

                break;
        }
    }

    return output;
}
