#include <iostream>
#include <string>
using namespace std;

#include "./src/parser.hpp"

int main(int argc, char const* argv[]) {
    if (argc == 1) {
        printf("Please input a source file.\n");
        return 1;
    }
    if (argc > 2) {
        printf("Invalid inputs being provided.\n");
        return 1;
    }
    string inputFilename = string(argv[1]);
    Parser* parser = initializeParser(inputFilename);
    runParser(parser);
    if (parser->lexerErrors.empty()) {
        unsigned int numTokens = parser->tokenList.size();
        for (unsigned int i = 0; i < numTokens; ++i) {
            parser->tokenList[i]->print();
        }
    } else {
        unsigned int numLexerErros = parser->lexerErrors.size();
        for (unsigned int i = 0; i < numLexerErros; ++i) {
            parser->lexerErrors[i]->print(parser->lexer->fileName);
        }
    }
    return 0;
}