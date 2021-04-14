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

    if (!parser->lexerErrors.empty()) {
        unsigned int numLexerErros = parser->lexerErrors.size();
        for (unsigned int i = 0; i < numLexerErros; ++i) {
            parser->lexerErrors[i]->print(parser->lexer->fileName);
        }
    }

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
    return 0;
}