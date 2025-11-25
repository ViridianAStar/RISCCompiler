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

    const STR text = "b => c;";
    const TVEC line = Tokenizer::lexLine(text, 0);
    for (const TVEC parsed = Parser::compressTokens(line); auto & i : parsed) {
        printf("%s\n", i.value.c_str());
    }

    printf("This compiler provides no guarantees of code functionality. Writing functional code is left to you, the user.\n");
    return 0;
}