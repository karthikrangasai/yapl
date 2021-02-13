#ifndef PARSER_H
#define PARSER_H
#include <fstream>
#include <iostream>
#include <vector>

#include "lexer.hpp"
// #include "token.cpp"
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Parser {
    Lexer* lexer;
    vector<Token*> tokenList;

    Parser(string filename) {
        lexer = initLexer(filename);
        tokenList = vector<Token*>();
    }
} Parser;

void printTokenMessage(Token* token);

void runParser(Parser* parser);

Parser* initializeParser(string filename);

#ifdef __cplusplus
}
#endif
#endif