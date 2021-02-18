#ifndef LEXER_H
#define LEXER_H
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <stack>

#include "token.hpp"
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

#define _NEWLINE '\n'
#define _INDENT '\t'

enum LEXER_ERROR_TYPE {
    STRAY_CHARACTER,
    INVALID_VAR_LITERAL,
    INVALID_FUNC_LITERAL,
    INVALID_INTEGER,
    INVALID_FLOAT,
    INVALID_STRING
};
typedef struct LexerError {
    unsigned int lineNumber;
    unsigned int columnNumber;
    LEXER_ERROR_TYPE errorType;
    string buffer;
    unsigned int errorLength;

    LexerError(unsigned int line_number, unsigned int column_number, LEXER_ERROR_TYPE error_type, string _buffer, unsigned int error_length) {
        lineNumber = line_number;
        columnNumber = column_number;
        errorType = error_type;
        buffer = _buffer;
        errorLength = error_length;
    }

    void print(string filename) {
        cout << filename << ":" << lineNumber << ":" << columnNumber << ": ";
        cout << "\033[1;31merror: \033[0m";
        if (errorType == STRAY_CHARACTER) {
            cout << "Character doesn't belong to the language.\n";
        } else if (errorType == INVALID_VAR_LITERAL) {
            cout << "Invalid variable literal.\n";
        } else if (errorType == INVALID_FUNC_LITERAL) {
            cout << "Invalid function literal.\n";
        } else if (errorType == INVALID_INTEGER) {
            cout << "Invalid integer format.\n";
        } else if (errorType == INVALID_FLOAT) {
            cout << "Invalid float format.\n";
        } else if (errorType == INVALID_STRING) {
            cout << "Invalid string format.\n";
        }
        cout << "  " << lineNumber << " | " << buffer << "\n";
        // cout << "  " << lineNumber << " | ";
        // for (unsigned int i = 0; i < columnNumber; ++i) {
        //     cout << " ";
        // }
        // cout << "^";
        // for (unsigned int i = 0; i < (errorLength - 1); ++i) {
        //     cout << "~";
        // }
        // cout << "\n";
    }
} LexerError;

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
    FLOAT_LITERAL,
    STRING_LITERAL
};

enum STRING_STATE {
    STRING_STATE_START,
    STRING_STATE_ONE,
    STRING_STATE_TWO,
    STRING_STATE_END,
    STRING_STATE_ERROR
};

enum INTEGER_STATE {
    INTEGER_STATE_START,
    INTEGER_STATE_ONE,
    INTEGER_STATE_TWO,
    INTEGER_STATE_END,
    INTEGER_STATE_TRAP
};

enum FLOAT_STATE {
    FLOAT_STATE_START,
    FLOAT_STATE_ONE,
    FLOAT_STATE_TWO,
    FLOAT_STATE_END,
    FLOAT_STATE_TRAP
};

enum IDENTIFIER_STATE {
    IDENTIFIER_STATE_START,
    IDENTIFIER_STATE_ONE,
    IDENTIFIER_STATE_TWO,
    IDENTIFIER_STATE_END,
    IDENTIFIER_STATE_TRAP
};

typedef struct Lexer {
    string fileName;
    fstream source_file;
    string buffer;
    unsigned int currPtr;
    unsigned int lookAheadPtr;
    unsigned int lineNumber;
    unsigned int bufferLen;

    STRING_STATE stringState;
    INTEGER_STATE integerState;
    FLOAT_STATE floatState;
    IDENTIFIER_STATE identifierState;
    LITERAL_STATE currentLiteralState;

    string identifierTokenBuffer;
    Token* currentToken;
    LEXER_STATE lexerState;
    LexerError* currentLexerError;

    stack<int> indentationStack;

    bool emptyBuffer;
    bool reachedEOF;

    Lexer(string file_name) {
        fileName = string(file_name);
        source_file.open(file_name, fstream::in);
        // buffer = "";
        currPtr = lookAheadPtr = 0;
        lineNumber = 0;
        bufferLen = 0;

        stringState = STRING_STATE_START;
        integerState = INTEGER_STATE_START;
        floatState = FLOAT_STATE_START;
        identifierState = IDENTIFIER_STATE_START;
        currentLiteralState = NO_LITERAL;

        identifierTokenBuffer = "";
        currentToken = NULL;
        lexerState = SEARCHING;
        currentLexerError = NULL;

        indentationStack = stack<int>();

        emptyBuffer = true;
        reachedEOF = false;
    }

    void reloadBuffer() {
        getline(source_file, buffer);
        bufferLen = buffer.size();
        ++lineNumber;
        reachedEOF = source_file.eof();
        emptyBuffer = (bufferLen == 0);
        while (emptyBuffer && !reachedEOF) {
            getline(source_file, buffer);
            bufferLen = buffer.size();
            emptyBuffer = (bufferLen == 0);
            ++lineNumber;
            reachedEOF = source_file.eof();
        }
        currPtr = lookAheadPtr = 0;
    }

    void updateCurrPtr(int n) {
        currPtr = lookAheadPtr + n;
    }

    void updateLookAheadPtr() {
        lookAheadPtr = currPtr;
    }

    bool isSearching() {
        return (lexerState == SEARCHING);
    }

    void foundToken() {
        lexerState = FOUND_TOKEN;
    }

    void resetGotToken() {
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