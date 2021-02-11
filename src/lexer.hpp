#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <iostream>
#include <stack>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdio.h>

#include "token.hpp"

#define MAX_INDENTATION 100

#define _NEWLINE '\n'
#define _INDENT '\t'
#define BLOCK_OPEN '{'
#define BLOCK_ClOSE '}'
#define PARENTHESES_OPEN '('
#define PARENTHESES_CLOSE ")"
#define COMMA_SEP ','
#define STATEMENT_SEP ';'
#define COMMENT_START '#'

typedef enum ERROR_TYPE {
    SYNTAX_ERROR,
    INDENTATION_ERROR,
    NEWLINE_ERROR
} ERROR_TYPE;

typedef struct Error {
    int line_number;
    ERROR_TYPE token;
    char* lexeme;
} Error;

void printError(Error error);

typedef struct lexer {
    fstream source_file;
    string buffer;
    int currPtr;
    int lookAheadPtr;
    int lineNumber;
    int bufferLen;
    bool gotToken;
    stack<int> indentStack;

    lexer(string filename) {
        source_file.open(filename, fstream::in);
        indentStack = stack<int>();
        currPtr = lookAheadPtr = lineNumber = -1;
        bufferLen = 0;
        gotToken = false;
    }

    void updateCurrPtr(int n) {
        currPtr = lookAheadPtr + n;
    }

    void updateLookAheadPtr() {
        lookAheadPtr = currPtr;
    }

    void foundToken() {
        gotToken = true;
    }

    void resetGotToken() {
        gotToken = false;
    }
} lexer;

bool isVariableIdentifier(string identifier);
bool isFunctionIdentifier(string identifier);

Token* getNextToken();

bool initLexer(string filename);

#ifdef __cplusplus
}
#endif
#endif