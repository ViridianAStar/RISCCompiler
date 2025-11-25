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

    const STR text0 = "int b; int c; int d; int e;";
    const STR text = "b => c;"; // passes! (expected)
    const STR text2 = "d = e;";
    VEC<TVEC> toks = {Tokenizer::lexLine(text0, 0), Tokenizer::lexLine(text, 1), Tokenizer::lexLine(text2, 2)};
    for (const auto& lines : Parser::parseFile(toks)) {
            for (const auto& working : lines) {
                STR printout = working.prettyPrint();
                printf("%s\n", printout.c_str());
            }
    }


    printf("This compiler provides no guarantees of code functionality. Writing functional code is left to you, the user.\n");
    return 0;
}