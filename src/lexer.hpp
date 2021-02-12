#ifndef LEXER_H
#define LEXER_H
#include <fstream>
#include <iostream>
#include <stack>

#include "token.hpp"
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

#define _NEWLINE '\n'
#define _INDENT '\t'

// typedef struct Error {
//     int line_number;
//     ERROR_TYPE token;
//     char* lexeme;
// } Error;

// void printError(Error error);
enum LITERAL_STATE {
    NO_LITERAL,
    VARIABLE_LITERAL,
    FUNCTION_LITERAL,
    INTEGER_LITERAL,
    FLOAT_LITERAL
};

enum ERROR_STATE {
    NO_ERROR,
    SYNTAX_ERROR,
    INDENTATION_ERROR,
    NEWLINE_ERROR
};

typedef struct lexer {
    string fileName;
    fstream source_file;
    string buffer;
    int currPtr;
    int lookAheadPtr;
    int lineNumber;
    int bufferLen;
    bool gotToken;
    stack<int> indentationStack;
    LITERAL_STATE currentLiteralState;
    string identifierTokenBuffer;
    ERROR_STATE errorState;
    bool emptyBuffer;
    bool eof;

    lexer(string file_name) {
        fileName = string(file_name);
        source_file.open(file_name, fstream::in);
        indentationStack = stack<int>();
        currPtr = lookAheadPtr = -1;
        lineNumber = 0;
        bufferLen = 0;
        gotToken = false;
        currentLiteralState = NO_LITERAL;
        identifierTokenBuffer = "";
        errorState = NO_ERROR;
        emptyBuffer = true;
        eof = false;
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

bool isKeyword(string identifier);
bool isVariableLiteral(string identifier);
bool isFunctionLiteral(string identifier);

Token* getNextToken();

bool initLexer(string file_name);

#ifdef __cplusplus
}
#endif
#endif