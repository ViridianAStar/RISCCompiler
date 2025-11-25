//
// Created by bowma on 11/5/2025.
//

#include "ASTNode.h"

STR ASTNode::prettyPrint() const {
    STR prettyNode;
    prettyNode.reserve(128);
    prettyNode.append("{\n");
    prettyNode.append("[Lin(" + std::to_string(line) + ") : Col(" + std::to_string(column) + ")]\n");
    switch (ASTNodeType Type = this->type) {
        case ASTNodeType::Variable:
            prettyNode.append("[Variable Node] (\n");
            break;
        case ASTNodeType::Definition:
            prettyNode.append("[Definition Node] (\n");
            break;
        case ASTNodeType::Operation:
            prettyNode.append("[Operation Node] (\n");
            break;
        case ASTNodeType::Control:
            prettyNode.append("[Control Node]\n (\n");
            break;
    }
    prettyNode.append("    [nodeName : " + nodeName + "]\n");
    prettyNode.append("    [value : " + value + "]\n");
    if (!children.empty()) {
        for (const auto& child : children) {
            prettyNode.append("    [" + child.nodeName + " : " + child.value + "]\n");
        }
    }
    prettyNode.append("  ) \n}\n");
    return prettyNode;
}

void ASTNode::addChild(const ASTNode& child) {
    children.push_back(child);
}
