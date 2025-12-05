#include "includes.h"
#include <elfio/elfio.hpp>

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

    const STR text0 = "1 + 1";
    const TVEC line = Tokenizer::lexLine(text0, 0);
    ANVEC parsedLine = Parser::parseLine(line);
    /*for (const ASTNode& ast_node : parsedLine) {
        printf("%s", ast_node.prettyPrint().c_str());
    }*/
    const ASTNode tree = AST::compressNodes(parsedLine);
    recursivePrintNodes(tree);

    printf("This compiler provides no guarantees of code functionality. Writing functional code is left to you, the user.\n");
    return 0;
}