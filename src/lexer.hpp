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

// typedef struct Error {
//     int line_number;
//     ERROR_TYPE token;
//     char* lexeme;
// } Error;

// void printError(Error error);
typedef enum IDENTIFIER_STATE {
    NON_IDENTIFIER_CHECK,
    IDENTIFIER_CHECK,  // In process
    // IDENTIFIER_END     // Done
} IDENTIFIER_STATE;

typedef enum FUNC_IDENTIFIER_STATE {
    NON_FUNC_IDENTIFIER_CHECK,
    FUNC_IDENTIFIER_START,
    FUNC_IDENTIFIER_CHECK  // In process
    // IDENTIFIER_END     // Done
} FUNC_IDENTIFIER_STATE;

typedef enum ERROR_STATE {
    NO_ERROR,
    SYNTAX_ERROR,
    INDENTATION_ERROR,
    NEWLINE_ERROR
} ERROR_STATE;

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
    bool spaceIndentation;
    bool keywordDetected;

    IDENTIFIER_STATE identifierState;
    FUNC_IDENTIFIER_STATE funcIdentifierState;
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
        gotToken = spaceIndentation = keywordDetected = false;
        identifierState = NON_IDENTIFIER_CHECK;
        funcIdentifierState = NON_FUNC_IDENTIFIER_CHECK;
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

    void spaceIndentationRequired() {
        spaceIndentation = true;
    }

    void spaceIndentationReset() {
        spaceIndentation = false;
    }

    bool isSpaceIndentationRequired() {
        return spaceIndentation;
    }

    void setKeywordDetected() {
        keywordDetected = true;
    }

    void resetKeywordDetected() {
        keywordDetected = false;
    }

    bool isKeywordDetected() {
        return keywordDetected;
    }
} lexer;

bool isKeyword(string identifier);
bool isVariableIdentifier(string identifier);
bool isFunctionIdentifier(string identifier);

Token* getNextToken();

bool initLexer(string file_name);

#ifdef __cplusplus
}
#endif
#endif