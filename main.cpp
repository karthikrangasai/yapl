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

    // generateParseTable(parser);

    runParser(parser);

    if (parser->syntaxErrors.empty()) {
        // print LMD
        fstream f("output.txt", fstream::out);
        for (vector<ppp> stackInput : parser->stackLMD) {
            for (auto it : stackInput) {
                if (it.first == ParserTokenType::TERMINAL)
                    f << terminal_names[it.second.terminal] << " ";
                else
                    f << non_terminal_names[it.second.nonTerminal] << " ";
            }
            f << endl
              << endl;
        }
        f.close();
    } else {
        for (SyntaxError e : parser->syntaxErrors) {
            e.print();
        }
    }

    // cout << "Syntax Errors:" << endl;
    // for (SyntaxError e : parser->syntaxErrors) {
    //     e.print();
    // }

    // cout << "--------------------------------------------------------------" << endl;
    // cout << "LMD:" << endl;
    // for (vector<ppp> stackInput : parser->stackLMD) {
    //     printStackProductions(stackInput);
    // }

    // if (parser->lexerErrors.empty()) {
    //     unsigned int numTokens = parser->tokenList.size();
    //     for (unsigned int i = 0; i < numTokens; ++i) {
    //         parser->tokenList[i]->print();
    //     }
    // } else {
    //     unsigned int numLexerErros = parser->lexerErrors.size();
    //     for (unsigned int i = 0; i < numLexerErros; ++i) {
    //         parser->lexerErrors[i]->print(parser->lexer->fileName);
    //     }
    // }
    return 0;
}