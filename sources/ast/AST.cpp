#include "./AST.h"

ASTNode AST::compressCollectionNode(const ANVEC &line, int &i) {
    if (i >= line.size() || line[i].type != ANT::Control || line[i].nodeName != "grouping start") throw AST_exception("Expected grouping start");

    ANVEC compress;
    int depth = 1;
    const int startIndex = i + 1;

    for (int j = startIndex; j < line.size(); j++) {
        const auto &working = line[j];

        if (working.type == ANT::Control) {
            if (working.nodeName == "grouping start") depth++;
            else if (working.nodeName == "grouping end") {
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

    return branchedNode;
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
        auto &node = line[i];

        if (node.type == ANT::Control && node.nodeName == "grouping start") {
            if (ASTNode branchNode = compressCollectionNode(line, i);
                branchNode.type == ASTNodeType::Line && branchNode.children.size() == 1) {

                output.addChild(branchNode.children[0]);
            } else {
                output.addChild(branchNode);
            }
            continue;
        }

        switch (node.type) {
            case ASTNodeType::File:
            case ASTNodeType::Branch:
            case ASTNodeType::Line:
                throw AST_exception("Lines cannot contain lines, files, or branches at this time! line ", node.line,
                                    "!");

            case ASTNodeType::Variable: {
                if (node.subType == ST::DataStatement || node.subType == ST::NameStatement) {
                    break;
                }
                if (node.subType == ST::TypeStatement) {
                    if (i + 3 >= line.size())
                        throw AST_exception("Incomplete statements on line: ", node.line, "!");

                    auto &next = line[i + 1];
                    auto &over = line[i + 2];
                    auto &final = line[i + 3];

                    if (next.subType == ST::NameStatement &&
                        over.subType == ST::AssignmentStatement) {
                        if (final.subType == ST::DataStatement || final.subType == ST::NameStatement) {
                            next.addChild(node);
                            over.addChild(next);
                            over.addChild(final);
                            output.addChild(over);
                            i += 3;
                        } else if (final.type == ANT::Control && final.nodeName == "grouping start") {
                            i += 2;
                            ASTNode branchNode = compressCollectionNode(line, i);
                            next.addChild(node);
                            over.addChild(next);
                            over.addChild(branchNode);
                            output.addChild(over);
                        }
                    } else throw AST_exception("Incomplete statements on line: ", node.line, "!");
                }
            }
            break;

            case ANT::Definition:
            case ANT::Control:

                if (node.subType == ST::Collection) {
                    ASTNode branchNode = compressCollectionNode(line, i);
                    output.addChild(branchNode);
                }
                break;

            case ANT::Operation:
                if (node.subType == ST::UnaryExpression) {
                    if (i + 1 < line.size()) {
                        if (auto &next = line[i + 1]; next.type == ANT::Variable &&
                                                      (next.subType == ST::NameStatement || next.subType ==
                                                       ST::DataStatement)) {
                            node.addChild(next);
                            output.addChild(node);
                            i += 1;
                        }
                    }
                } else if (node.subType == ST::BinaryExpression) {
                    if (i > 0 && i + 1 < line.size()) {
                        auto &previous = line[i - 1];

                        if (auto &next = line[i + 1];
                            (previous.subType == ST::DataStatement || previous.subType == ST::NameStatement) &&
                            (next.subType == ST::NameStatement || next.subType == ST::DataStatement)) {
                            node.addChild(previous);
                            node.addChild(next);
                            output.addChild(node);
                            i += 1;
                        }
                    }
                }
                break;
        }
    }

    return output;
}
