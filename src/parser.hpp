#ifndef PARSER_H
#define PARSER_H

#include "lexer.cpp"
#include "token.cpp"

void printTokenMessage(Token* token);

bool startParser(char* filename);

#endif