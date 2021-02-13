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
enum LEXER_STATE {
    SEARCHING,
    FOUND_TOKEN,
    COMMENT_FOUND,
    ERROR_OCCURED
};

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
typedef struct Lexer {
    string fileName;
    fstream source_file;
    string buffer;
    int currPtr;
    int lookAheadPtr;
    int lineNumber;
    int bufferLen;

    bool gotToken;
    Token* currentToken;
    LEXER_STATE lexerState;

    stack<int> indentationStack;

    LITERAL_STATE currentLiteralState;
    string identifierTokenBuffer;
    ERROR_STATE errorState;
    bool emptyBuffer;
    bool reachedEOF;

    Lexer(string file_name) {
        fileName = string(file_name);
        source_file.open(file_name, fstream::in);
        indentationStack = stack<int>();
        currPtr = lookAheadPtr = -1;
        lineNumber = 0;
        bufferLen = 0;
        gotToken = false;
        currentToken = NULL;
        lexerState = SEARCHING;
        currentLiteralState = NO_LITERAL;
        identifierTokenBuffer = "";
        errorState = NO_ERROR;
        emptyBuffer = true;
        reachedEOF = false;
    }

    void reloadBuffer() {
        getline(source_file, buffer);
        ++lineNumber;
        while (buffer.size() == 0) {
            getline(source_file, buffer);
            ++lineNumber;
        }
        bufferLen = buffer.size();
        currPtr = lookAheadPtr = 0;
    }

    void updateCurrPtr(int n) {
        currPtr = lookAheadPtr + n;
    }

    void updateLookAheadPtr() {
        lookAheadPtr = currPtr;
    }

    void foundToken() {
        gotToken = true;
        lexerState = FOUND_TOKEN;
    }

    void resetGotToken() {
        gotToken = false;
        lexerState = SEARCHING;
    }

    void errorOccured() {
        lexerState = ERROR_OCCURED;
    }

    void reseterrorOccured() {
        lexerState = SEARCHING;
    }
} Lexer;

bool isKeyword(string identifier);
bool isVariableLiteral(string identifier);
bool isFunctionLiteral(string identifier);

void getNextToken(Lexer* lexer);

Lexer* initLexer(string file_name);

#ifdef __cplusplus
}
#endif
#endif