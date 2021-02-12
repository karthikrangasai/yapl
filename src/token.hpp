/* Token types */
#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

typedef enum TOKEN_TYPE {
    BITWISE_NOT = 0,
    BITWISE_AND = 1,
    BITWISE_OR = 2,
    LEFT_SHIFT = 3,
    RIGHT_SHIFT = 4,
    MODULO = 5,
    DIVIDE = 6,
    MULTIPLY = 7,
    ADD = 8,
    SUBTRACT = 9,
    LESS_OR_EQUAL = 10,
    GREATER_OR_EQUAL = 11,
    LESS = 12,
    GREATER = 13,
    EQUAL = 14,
    NOTEQUAL = 15,
    BINDING = 16,
    ASSIGNMENT = 17,
    LEFT_PAREN = 18,
    RIGHT_PAREN = 19,
    LEFT_SQUARE_BR = 20,
    RIGHT_SQUARE_BR = 21,
    LEFT_FLOWER_BR = 22,
    RIGHT_FLOWER_BR = 23,
    COMMA = 24,
    SEMI_COLON = 25,
    COLON = 26,
    NEWLINE = 27,
    INDENT = 28,
    DEDENT = 29,
    KEYWORD = 30,
    VAR_IDENTIFIER = 31,
    AT_THE_RATE = 32,
    FUNC_IDENTIFIER = 33,
    INTEGER = 34,
    FLOAT = 35,
    COMMENT = 36,
    SPACE = 37,
    ERROR = 420
} TOKEN_TYPE;

// #define ERROR 420
#define ENDMARKER 0

#define NUM_TOKENS 38
#define NT_OFFSET 256

const char* const token_names[38] = {
    "BITWISE_NOT",
    "BITWISE_AND",
    "BITWISE_OR",
    "LEFT_SHIFT",
    "RIGHT_SHIFT",
    "MODULO",
    "DIVIDE",
    "MULTIPLY",
    "ADD",
    "SUBTRACT",
    "LESS_OR_EQUAL",
    "GREATER_OR_EQUAL",
    "LESS",
    "GREATER",
    "EQUAL",
    "NOTEQUAL",
    "BINDING",
    "ASSIGNMENT",
    "LEFT_PAREN",
    "RIGHT_PAREN",
    "LEFT_SQUARE_BR",
    "RIGHT_SQUARE_BR",
    "LEFT_FLOWER_BR",
    "RIGHT_FLOWER_BR",
    "COMMA",
    "SEMI_COLON",
    "COLON",
    "NEWLINE",
    "INDENT",
    "DEDENT",
    "KEYWORD",
    "VAR_IDENTIFIER",
    "AT_THE_RATE",
    "FUNC_IDENTIFIER",
    "INTEGER",
    "FLOAT",
    "COMMENT",
    "SPACE",
};

#define NUM_KEYWORDS 17
const string keywords[17] = {
    "void",
    "boolean",
    "true",
    "false",
    "integer",
    "float",
    "if",
    "else",
    "for",
    "while",
    "end",
    "function",
    "end_definition",
    "@main",
    "break",
    "continue",
    "return",
};

typedef struct Token {
    int line_number;
    TOKEN_TYPE token;
    string lexeme;

    Token(int lineNumber, TOKEN_TYPE token_type, string lexeme_string) {
        line_number = lineNumber;
        token = token_type;
        lexeme = string(lexeme_string);
    }
} Token;

TOKEN_TYPE getTokenTypeUsingOneChar(int character);
TOKEN_TYPE getTokenTypeUsingTwoChar(int, int);
TOKEN_TYPE getTokenTypeUsingThreeChar(int, int, int);
bool belongsToFirstIdentifierAlphabet(int character);
bool belongsToIdentifierAlphabet(int character);
const string getTokenName(int token_code);

#ifdef __cplusplus
}
#endif

#endif