#include "includes.h"
#include <elfio/elfio.hpp>

/*void recursivePrintNodes(const ASTNode& ast_node) {
    printf("%s", ast_node.prettyPrint().c_str());
    if (!ast_node.children.empty()) {
        for (const ASTNode& child : ast_node.children) {
            recursivePrintNodes(child);
        }
    }
}*/

int main() {
    printf("This compiler provides no guarantees of code functionality. Writing functional code is left to you, the user.\n");

    const STR text0 = "int i = (1 + (2 + (3 + 4)));"; // not working right currently
    const TVEC line = Tokenizer::lexLine(text0, 0);
    printf("\nStage1\n");
    for (const Token& token : line) {
        printf(("Token: " + token.value + "\n").c_str());
    }
    printf("\nStage1\n");
    printf("\nStage2\n");
    ANVEC parsedLine = Parser::parseLine(line);
    printf("Successfully made nodes");
    // for (const ASTNode& ast_node : parsedLine) {
    //     printf("%s", ast_node.prettyPrint(0).c_str());
    // }
    printf("\nStage2\n");
    printf("\nStage3\n");
    const ASTNode tree = AST::compressNodes(parsedLine);
    //recursivePrintNodes(tree);
    printf(tree.prettyPrint(0).c_str());
    printf("\nStage3\n");

    printf("This compiler provides no guarantees of code functionality. Writing functional code is left to you, the user.\n");
    return 0;
}