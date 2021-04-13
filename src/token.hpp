/* Token types */
#ifndef TOKEN_H
#define TOKEN_H

#include <iomanip>
#include <iostream>
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
    LOGICAL_NOT = 14,
    LOGICAL_AND = 15,
    LOGICAL_OR = 16,
    EQUAL = 17,
    NOTEQUAL = 18,
    BINDING = 19,
    ASSIGNMENT = 20,
    LEFT_PAREN = 21,
    RIGHT_PAREN = 22,
    LEFT_SQUARE_BR = 23,
    RIGHT_SQUARE_BR = 24,
    LEFT_FLOWER_BR = 25,
    RIGHT_FLOWER_BR = 26,
    COMMA = 27,
    SEMI_COLON = 28,
    COLON = 29,
    NEWLINE = 30,
    INDENT = 31,
    DEDENT = 32,
    KEYWORD = 33,
    VAR_IDENTIFIER = 34,
    AT_THE_RATE = 35,
    FUNC_IDENTIFIER = 36,
    INTEGER_TOKEN = 37,
    FLOAT_TOKEN = 38,
    COMMENT = 39,
    SPACE = 40,
    TAB = 41,
    PRE_INCEREMENT = 42,
    PRE_DECREMENT = 43,
    STRING_TOKEN = 44,
    ERROR = 420
} TOKEN_TYPE;

// #define ERROR 420
#define ENDMARKER 0

#define NUM_TOKENS 38
#define NT_OFFSET 256

const char* const token_names[45] = {
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
    "LOGICAL_NOT",
    "LOGICAL_AND",
    "LOGICAL_OR",
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
    "TAB",
    "PRE_INCEREMENT",
    "PRE_DECREMENT",
    "STRING",
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
    // "@main",
    "break",
    "continue",
    "return",
};

typedef struct Token {
    unsigned int line_number;
    TOKEN_TYPE token;
    string token_name;
    string lexeme;

    Token(int lineNumber, TOKEN_TYPE token_type, string lexeme_string) {
        line_number = lineNumber;
        token = token_type;
        token_name = token_names[token_type];
        lexeme = string(lexeme_string);
    }

    void print() {
        cout << "Line Number: " << setw(3) << line_number << ", ";
        cout << "Token Code: " << setw(3) << token << ", ";
        cout << "Token type: " << left << setw(16) << token_name << right << ", ";
        cout << "Lexeme: " << lexeme << "\n";
    }
} Token;

TOKEN_TYPE getTokenTypeUsingOneChar(int character);
TOKEN_TYPE getTokenTypeUsingTwoChar(int, int);
TOKEN_TYPE getTokenTypeUsingThreeChar(int, int, int);

bool isNumber(int character);
bool belongsToFirstCharacterSetOfIndentifierRE(char character);
bool belongsToIndentifierCharacterSet(char character);
bool belongstoNumbersCharacterSet(char character);

#ifdef __cplusplus
}
#endif

#endif