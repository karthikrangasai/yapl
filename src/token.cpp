#include "token.hpp"

TOKEN_TYPE getTokenTypeUsingOneChar(int character) {
    switch (character) {
        case '~':
            return BITWISE_NOT;
            break;
        case '&':
            return BITWISE_AND;
            break;
        case '|':
            return BITWISE_OR;
            break;

        case '%':
            return MODULO;
            break;
        case '/':
            return DIVIDE;
            break;
        case '*':
            return MULTIPLY;
            break;
        case '+':
            return ADD;
            break;
        case '-':
            return SUBTRACT;
            break;

        case '<':
            return LESS;
            break;
        case '>':
            return GREATER;
            break;
        case '=':
            return EQUAL;
            break;

        case '!':
            return LOGICAL_NOT;
            break;

        case '(':
            return LEFT_PAREN;
            break;
        case ')':
            return RIGHT_PAREN;
            break;
        case '[':
            return LEFT_SQUARE_BR;
            break;
        case ']':
            return RIGHT_SQUARE_BR;
            break;
        case '{':
            return LEFT_FLOWER_BR;
            break;
        case '}':
            return RIGHT_FLOWER_BR;
            break;
        case ',':
            return COMMA;
            break;
        case ';':
            return SEMI_COLON;
            break;
        case ':':
            return COLON;
            break;
        case '#':
            return COMMENT;
            break;
        case '@':
            return AT_THE_RATE;
            break;
        case ' ':
            return SPACE;
            break;
        case '\t':
            return TAB;
            break;
        case '\n':
            return NEWLINE;
            break;
        default:
            return ERROR;
            break;
    }
    return ERROR;
}

TOKEN_TYPE getTokenTypeUsingTwoChar(int char1, int char2) {
    switch (char1) {
        case '&': {
            switch (char2) {
                case '&':
                    return LOGICAL_AND;
            }
        }

        case '|': {
            switch (char2) {
                case '|':
                    return LOGICAL_OR;
            }
        }

        case '<': {
            switch (char2) {
                case '<':
                    return LEFT_SHIFT;
                case '=':
                    return LESS_OR_EQUAL;
            }
        }

        case '>': {
            switch (char2) {
                case '>':
                    return RIGHT_SHIFT;
                case '=':
                    return GREATER_OR_EQUAL;
            }
        }

        case '+': {
            switch (char2) {
                case '+':
                    return PRE_INCEREMENT;
            }
        }

        case '-': {
            switch (char2) {
                case '-':
                    return PRE_DECREMENT;
            }
        }

        case ':': {
            switch (char2) {
                case ':':
                    return BINDING;
                case '=':
                    return ASSIGNMENT;
            }
        }

        default:
            return ERROR;
    }
    return ERROR;
}
TOKEN_TYPE getTokenTypeUsingThreeChar(int char1, int char2, int char3) {
    if (char1 == '=') {
        if (char2 == '/') {
            if (char3 == '=') {
                return NOTEQUAL;
            } else {
                return ERROR;
            }
        }
    }
    return ERROR;
}

bool isNumber(int character) {  //
    return (character >= '0' && character <= '9');
}

bool belongsToFirstCharacterSetOfIndentifierRE(char character) {
    return ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z') || (character == '_'));
}

bool belongsToIndentifierCharacterSet(char character) {
    return ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z') || (character >= '0' && character <= '9') || (character == '_'));
}

bool belongstoNumbersCharacterSet(char character) {
    return ((character >= '0' && character <= '9') || (character == '.'));
}
