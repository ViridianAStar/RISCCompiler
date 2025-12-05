//
// Created by bowma on 11/5/2025.
//

#include "ASTNode.h"
#include "../../Exceptions/AST_exception.cpp"

STR ASTNode::prettyPrint(const int indentLevel = 0) const {
    STR prettyNode;
    prettyNode.reserve(256);

    const STR indent(indentLevel * 4, ':');

    prettyNode.append(indent + "{\n");
    prettyNode.append(indent + "[Line: " + std::to_string(line) + ", Col: " + std::to_string(column) + "]\n");

    switch (ASTNodeType Type = this->type) {
        case ASTNodeType::Variable:
            prettyNode.append(indent + "[Variable Node] (" + nodeName + " : " + value + ")\n");
            break;
        case ASTNodeType::Definition:
            prettyNode.append(indent + "[Definition Node] (" + nodeName + " : " + value + ")\n");
            break;
        case ASTNodeType::Operation:
            prettyNode.append(indent + "[Operation Node] (" + nodeName + " : " + value + ")\n");
            break;
        case ASTNodeType::Control:
            prettyNode.append(indent + "[Control Node] (" + nodeName + ")\n");
            break;
        case ASTNodeType::File:
            prettyNode.append(indent + "[File Node]\n");
            break;
        case ASTNodeType::Line:
            prettyNode.append(indent + "[Line Node]\n");
            break;
        case ASTNodeType::Branch:
            prettyNode.append(indent + "[Branch Node]\n");
            break;
    }


    for (const auto& child : children) {
        prettyNode.append(indent + "::[>\n");
        prettyNode.append(child.prettyPrint(indentLevel + 1));
        prettyNode.append(indent + "::<]\n");
    }

    prettyNode.append(indent + "}\n");
    return prettyNode;
}



void ASTNode::addChild(const ASTNode& child) {
    children.push_back(child);
}
