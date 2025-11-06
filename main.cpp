#include "includes.h"

void recursivePrintNodes(const ASTNode& ast_node) {
    printf("%s", ast_node.prettyPrint().c_str());
    if (!ast_node.children.empty()) {
        for (const ASTNode& child : ast_node.children) {
            recursivePrintNodes(child);
        }
    }
}

int main() {
    printf("This compiler provides no guarantees of code functionality. Writing functional code is left to you, the user.\n");

    const auto testChild1 = ASTNode(ASTNodeType::Control,0, 3, "Control Flow", "End of line");
    const auto testChild2 = ASTNode(ASTNodeType::Operation, 0, 2,"Assignment", "42");
    const auto testChild = ASTNode(ASTNodeType::Variable, 0, 1,"Integer Variable", "A", {testChild1, testChild2});
    const auto testNode = ASTNode(ASTNodeType::Definition, 0, 0,"Method", "entry", {testChild});

    recursivePrintNodes(testNode);


    printf("This compiler provides no guarantees of code functionality. Writing functional code is left to you, the user.\n");
    return 0;
}