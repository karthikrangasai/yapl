#include "lexer.hpp"

#include <iostream>
#include <sstream>
#include <string>
using namespace std;

// void printError(Error error) {}
bool isKeyword(string identifier) {
    for (int i = 0; i < NUM_KEYWORDS; ++i) {
        if (identifier == keywords[i]) {
            return true;
        }
    }
    return false;
}

void variable_start_state(char character, int* state) {
    if ((character >= 'a' and character <= 'z') or (character >= 'A' and character <= 'Z'))
        *state = 1;
    else
        *state = -1;  // trap state
}
void variable_state_1(char character, int* state) {
    if ((character >= 'a' and character <= 'z') or (character >= 'A' and character <= 'Z') or (character >= '0' and character <= '9') or (character == '_'))
        *state = 1;
    else
        *state = -1;  // trap state
}
// Test RE [a-zA-Z][_a-zA-Z0-9]*
bool isVariableLiteral(string identifier) {
    int state = 0;
    for (int i = 0; i < (int)identifier.size(); ++i) {
        if (state == 0) {
            variable_start_state(identifier[i], &state);
        } else if (state == 1) {
            variable_state_1(identifier[i], &state);
        } else if (state == -1) {  // trap state
            return false;
        }
    }
    return (state == 1);
}

// Test RE @[a-zA-Z][_a-zA-Z0-9]*
bool isFunctionLiteral(string identifier) {
    if (identifier == "@") {
        return false;
    }
    return (identifier[0] == '@') && isVariableLiteral(identifier.substr(1, identifier.size() - 1));
}

void integerREStartStateEvaluator(char character, int* state) {
    if (character >= '1' and character <= '9')
        *state = 1;
    else
        *state = -1;  // trap state
}
void integerREStateEvaluator(char character, int* state) {
    if (character >= '0' and character <= '9')
        *state = 1;
    else
        *state = -1;  // trap state
}
bool isIntegerLiteral(Lexer* lexer, string identifier) {
    if (identifier == "0") {
        return true;
    }
    int state = 0;
    for (int i = 0; i < (int)identifier.size(); i++) {
        if (state == 0) {
            integerREStartStateEvaluator(identifier[i], &state);
        } else if (state == 1) {
            integerREStateEvaluator(identifier[i], &state);
        } else if (state == -1) {  // trap state
            return false;
        }
    }
    return (state == 1);
}

void floatREStartStateEvaluator(char character, int* state) {
    if (character == '0') {
        *state = 4;
    } else if (character >= '1' and character <= '9') {
        *state = 5;
    } else {
        *state = -1;
    }
}
void state_2(char character, int* state) {
    if (character >= '1' and character <= '9') {
        *state = 2;
    } else if (character == '0') {
        *state = 3;
    } else {
        *state = -1;
    }
}
void state_3(char character, int* state) {
    if (character == '0') {
        *state = 3;
    } else if (character >= '1' and character <= '9') {
        *state = 2;
    } else {
        *state = -1;
    }
}
void state_4(char character, int* state) {
    if (character == '.') {
        *state = 6;
    } else {
        *state = -1;
    }
}
void state_5(char character, int* state) {
    if (character == '.') {
        *state = 6;
    } else if (character >= '0' and character <= '9') {
        *state = 5;
    } else {
        *state = -1;
    }
}
void state_6(char character, int* state) {
    if (character >= '0' and character <= '9') {
        *state = 2;
    } else {
        *state = -1;
    }
}
bool isFloatLiteral(Lexer* lexer, string identifier) {
    int state = 1;
    for (int i = 0; i < (int)identifier.size(); i++) {
        if (state == 1) {
            floatREStartStateEvaluator(identifier[i], &state);
        } else if (state == 2) {  // FINAL STATE
            state_2(identifier[i], &state);
        } else if (state == 3) {
            state_3(identifier[i], &state);
        } else if (state == 4) {
            state_4(identifier[i], &state);
        } else if (state == 5) {
            state_5(identifier[i], &state);
        } else if (state == 6) {
            state_6(identifier[i], &state);
        } else if (state == -1) {  // TRAP STATE
            return false;
        }
    }
    return (state == 2);
}

bool isValidLiteral(Lexer* lexer) {
    string identifier = lexer->identifierTokenBuffer;
    bool isValid = true;
    switch (lexer->currentLiteralState) {
        case VARIABLE_LITERAL: {
            isValid = isVariableLiteral(identifier);
            break;
        }
        case FUNCTION_LITERAL: {
            isValid = isFunctionLiteral(identifier);
            break;
        }
        case INTEGER_LITERAL: {
            isValid = isIntegerLiteral(lexer, identifier);
            break;
        }
        case FLOAT_LITERAL: {
            isValid = isFloatLiteral(lexer, identifier);
            break;
        }
        case STRING_LITERAL:
        case NO_LITERAL: {
            lexer->lexerState = ERROR_OCCURED;
            break;
        }
    }
    return isValid;
}

Token* getAppropriateToken(Lexer* lexer) {
    switch (lexer->currentLiteralState) {
        case VARIABLE_LITERAL:
        case FUNCTION_LITERAL: {
            if (isKeyword(lexer->identifierTokenBuffer)) {
                return new Token(lexer->lineNumber, KEYWORD, string(lexer->identifierTokenBuffer));
            }
            return new Token(
                lexer->lineNumber,
                (lexer->currentLiteralState == VARIABLE_LITERAL) ? VAR_IDENTIFIER : FUNC_IDENTIFIER,
                string(lexer->identifierTokenBuffer));
        }
        case INTEGER_LITERAL: {
            return new Token(lexer->lineNumber, INTEGER_TOKEN, string(lexer->identifierTokenBuffer));
        }
        case FLOAT_LITERAL: {
            return new Token(lexer->lineNumber, FLOAT_TOKEN, string(lexer->identifierTokenBuffer));
        }
        case STRING_LITERAL:
        case NO_LITERAL: {
            lexer->lexerState = ERROR_OCCURED;
            break;
        }
    }
    return NULL;
}

/*	function: _getNextTokenHelper()
	We have to read the buffer character by character
	Then for each char:
		if char is ":
			Maybe string literal
		else if char belongs to [a-zA-Z]:
			Maybe an identifier
		else if char belongs to [0-9]:
			Maybe an identifier
		else:
			Any other token like operators
		
		then check str[currPtr, lookAheadPtr]
		Then find full identifer and return IDENTIFIER / KEYWORD token
		else:
			Find it's TOKEN_TYPE and act accordingly
	Don't forget to update the lookAheadPtr.
*/
void _getNextTokenHelper(Lexer* lexer) {
    string buffer = lexer->buffer;

    while (lexer->isSearching()) {
        // Not sure what we are going to see
        if (lexer->currentLiteralState == NO_LITERAL) {
            if (buffer[lexer->lookAheadPtr] == '\"') {  // string data type
                lexer->currentLiteralState = STRING_LITERAL;
                lexer->stringState = STRING_STATE_ONE;
                lexer->identifierTokenBuffer = buffer.substr(lexer->currPtr, 1);
                ++lexer->lookAheadPtr;
            } else if (belongsToFirstCharacterSetOfIndentifierRE(buffer[lexer->lookAheadPtr])) {  // Checks for RE: [a-zA-Z]
                ++lexer->lookAheadPtr;
                while (belongsToIndentifierCharacterSet(buffer[lexer->lookAheadPtr])) {  // Keep looking until [_a-zA-Z0-9]
                    ++lexer->lookAheadPtr;
                }
                lexer->identifierTokenBuffer = buffer.substr(lexer->currPtr, (lexer->lookAheadPtr - lexer->currPtr));
                lexer->currentLiteralState = VARIABLE_LITERAL;
            } else if (isNumber(buffer[lexer->lookAheadPtr])) {  // check for [0-9]
                ++lexer->lookAheadPtr;
                while (belongstoNumbersCharacterSet(buffer[lexer->lookAheadPtr])) {  // Keep looking until [.0-9]
                    ++lexer->lookAheadPtr;
                }
                lexer->identifierTokenBuffer = buffer.substr(lexer->currPtr, (lexer->lookAheadPtr - lexer->currPtr));
                if (lexer->identifierTokenBuffer.find('.') != string::npos) {
                    lexer->currentLiteralState = FLOAT_LITERAL;
                } else {
                    lexer->currentLiteralState = INTEGER_LITERAL;
                }
            } else {
                TOKEN_TYPE token_type = getTokenTypeUsingOneChar(buffer[lexer->lookAheadPtr]);
                switch (token_type) {
                    case AT_THE_RATE: {
                        ++lexer->lookAheadPtr;
                        if (belongsToFirstCharacterSetOfIndentifierRE(buffer[lexer->lookAheadPtr])) {
                            ++lexer->lookAheadPtr;
                            while (belongsToIndentifierCharacterSet(buffer[lexer->lookAheadPtr])) {
                                ++lexer->lookAheadPtr;
                            }
                            lexer->identifierTokenBuffer = buffer.substr(lexer->currPtr, (lexer->lookAheadPtr - lexer->currPtr));
                            lexer->currentLiteralState = FUNCTION_LITERAL;
                        } else {
                            // check for just '@'
                            lexer->currentLexerError = new LexerError(lexer->lineNumber, lexer->currPtr, INVALID_FUNC_LITERAL, lexer->buffer, lexer->identifierTokenBuffer.size());
                            lexer->errorOccured();
                            lexer->identifierTokenBuffer.clear();
                            lexer->currentLiteralState = NO_LITERAL;
                            lexer->updateCurrPtr(0);
                        }
                        break;
                    }

                    case LESS: {  // '<' => Maybe '<<' or '<=' or just simply '<'

                        // Check for '<<' or '<='
                        if ((lexer->lookAheadPtr + 1) < lexer->bufferLen) {
                            TOKEN_TYPE new_token_type = getTokenTypeUsingTwoChar(buffer[lexer->lookAheadPtr], buffer[lexer->lookAheadPtr + 1]);
                            if (new_token_type == LEFT_SHIFT || new_token_type == LESS_OR_EQUAL) {
                                lexer->currentToken = new Token(lexer->lineNumber, new_token_type, buffer.substr(lexer->lookAheadPtr, 2), lexer->currPtr, lexer->buffer, buffer.substr(lexer->lookAheadPtr, 2).size(), lexer->fileName);
                                lexer->updateCurrPtr(2);
                                lexer->updateLookAheadPtr();
                                lexer->foundToken();
                                break;  // Break from swtich
                            }
                        }

                        // '<' only
                        lexer->currentToken = new Token(lexer->lineNumber, token_type, buffer.substr(lexer->lookAheadPtr, 1), lexer->currPtr, lexer->buffer, buffer.substr(lexer->lookAheadPtr, 1).size(), lexer->fileName);
                        lexer->updateCurrPtr(1);
                        lexer->updateLookAheadPtr();
                        lexer->foundToken();
                        break;
                    }

                    case GREATER: {  // '>' => Maybe '>>' or '>=' or just simply '>'

                        // Check for '>>' or '>='
                        if ((lexer->lookAheadPtr + 1) < lexer->bufferLen) {
                            TOKEN_TYPE new_token_type = getTokenTypeUsingTwoChar(buffer[lexer->lookAheadPtr], buffer[lexer->lookAheadPtr + 1]);
                            if (new_token_type == RIGHT_SHIFT || new_token_type == GREATER_OR_EQUAL) {
                                lexer->currentToken = new Token(lexer->lineNumber, new_token_type, buffer.substr(lexer->lookAheadPtr, 2), lexer->currPtr, lexer->buffer, buffer.substr(lexer->lookAheadPtr, 2).size(), lexer->fileName);
                                lexer->updateCurrPtr(2);
                                lexer->updateLookAheadPtr();
                                lexer->foundToken();
                                break;
                            }
                        }

                        // '>' only
                        lexer->currentToken = new Token(lexer->lineNumber, token_type, buffer.substr(lexer->lookAheadPtr, 1), lexer->currPtr, lexer->buffer, buffer.substr(lexer->lookAheadPtr, 1).size(), lexer->fileName);
                        lexer->updateCurrPtr(1);
                        lexer->updateLookAheadPtr();
                        lexer->foundToken();
                        break;
                    }

                    case ADD: {  // ':' => Maybe '++'

                        // Check for '++'
                        if ((lexer->lookAheadPtr + 1) < lexer->bufferLen) {
                            TOKEN_TYPE new_token_type = getTokenTypeUsingTwoChar(buffer[lexer->lookAheadPtr], buffer[lexer->lookAheadPtr + 1]);
                            if (new_token_type == PRE_INCEREMENT) {
                                lexer->currentToken = new Token(lexer->lineNumber, new_token_type, buffer.substr(lexer->lookAheadPtr, 2), lexer->currPtr, lexer->buffer, buffer.substr(lexer->lookAheadPtr, 2).size(), lexer->fileName);
                                lexer->updateCurrPtr(2);
                                lexer->updateLookAheadPtr();
                                lexer->foundToken();
                                break;
                            }
                        }

                        // '+' only
                        lexer->currentToken = new Token(lexer->lineNumber, token_type, buffer.substr(lexer->lookAheadPtr, 1), lexer->currPtr, lexer->buffer, buffer.substr(lexer->lookAheadPtr, 1).size(), lexer->fileName);
                        lexer->updateCurrPtr(1);
                        lexer->updateLookAheadPtr();
                        lexer->foundToken();
                        break;
                    }

                    case SUBTRACT: {  // '-' => Maybe '--'

                        // Check for '--'
                        if ((lexer->lookAheadPtr + 1) < lexer->bufferLen) {
                            TOKEN_TYPE new_token_type = getTokenTypeUsingTwoChar(buffer[lexer->lookAheadPtr], buffer[lexer->lookAheadPtr + 1]);
                            if (new_token_type == PRE_DECREMENT) {
                                lexer->currentToken = new Token(lexer->lineNumber, new_token_type, buffer.substr(lexer->lookAheadPtr, 2), lexer->currPtr, lexer->buffer, buffer.substr(lexer->lookAheadPtr, 2).size(), lexer->fileName);
                                lexer->updateCurrPtr(2);
                                lexer->updateLookAheadPtr();
                                lexer->foundToken();
                                break;
                            }
                        }

                        // '-' only
                        lexer->currentToken = new Token(lexer->lineNumber, token_type, buffer.substr(lexer->lookAheadPtr, 1), lexer->currPtr, lexer->buffer, buffer.substr(lexer->lookAheadPtr, 1).size(), lexer->fileName);
                        lexer->updateCurrPtr(1);
                        lexer->updateLookAheadPtr();
                        lexer->foundToken();
                        break;
                    }

                    case BITWISE_AND: {  // '&' => Maybe '&&'

                        // Check for '&&'
                        if ((lexer->lookAheadPtr + 1) < lexer->bufferLen) {
                            TOKEN_TYPE new_token_type = getTokenTypeUsingTwoChar(buffer[lexer->lookAheadPtr], buffer[lexer->lookAheadPtr + 1]);
                            if (new_token_type == LOGICAL_AND) {
                                lexer->currentToken = new Token(lexer->lineNumber, new_token_type, buffer.substr(lexer->lookAheadPtr, 2), lexer->currPtr, lexer->buffer, buffer.substr(lexer->lookAheadPtr, 2).size(), lexer->fileName);
                                lexer->updateCurrPtr(2);
                                lexer->updateLookAheadPtr();
                                lexer->foundToken();
                                break;
                            }
                        }

                        // '&' only
                        lexer->currentToken = new Token(lexer->lineNumber, token_type, buffer.substr(lexer->lookAheadPtr, 1), lexer->currPtr, lexer->buffer, buffer.substr(lexer->lookAheadPtr, 1).size(), lexer->fileName);
                        lexer->updateCurrPtr(1);
                        lexer->updateLookAheadPtr();
                        lexer->foundToken();
                        break;
                    }

                    case BITWISE_OR: {  // '|' => Maybe '||'

                        // Check for '|'
                        if ((lexer->lookAheadPtr + 1) < lexer->bufferLen) {
                            TOKEN_TYPE new_token_type = getTokenTypeUsingTwoChar(buffer[lexer->lookAheadPtr], buffer[lexer->lookAheadPtr + 1]);
                            if (new_token_type == LOGICAL_OR) {
                                lexer->currentToken = new Token(lexer->lineNumber, new_token_type, buffer.substr(lexer->lookAheadPtr, 2), lexer->currPtr, lexer->buffer, buffer.substr(lexer->lookAheadPtr, 2).size(), lexer->fileName);
                                lexer->updateCurrPtr(2);
                                lexer->updateLookAheadPtr();
                                lexer->foundToken();
                                break;
                            }
                        }

                        // '|' only
                        lexer->currentToken = new Token(lexer->lineNumber, token_type, buffer.substr(lexer->lookAheadPtr, 1), lexer->currPtr, lexer->buffer, buffer.substr(lexer->lookAheadPtr, 1).size(), lexer->fileName);
                        lexer->updateCurrPtr(1);
                        lexer->updateLookAheadPtr();
                        lexer->foundToken();
                        break;
                    }

                    case EQUAL: {  // '=' => Maybe '=/=' or just simply '='

                        // Check for '=/=' TOKEN
                        if (((lexer->lookAheadPtr + 1) < lexer->bufferLen && ((lexer->lookAheadPtr + 2) < lexer->bufferLen))) {
                            TOKEN_TYPE new_token_type = getTokenTypeUsingThreeChar(buffer[lexer->lookAheadPtr], buffer[lexer->lookAheadPtr + 1], buffer[lexer->lookAheadPtr + 2]);
                            if (new_token_type == NOTEQUAL) {
                                lexer->currentToken = new Token(lexer->lineNumber, new_token_type, buffer.substr(lexer->lookAheadPtr, 3), lexer->currPtr, lexer->buffer, buffer.substr(lexer->lookAheadPtr, 3).size(), lexer->fileName);
                                lexer->updateCurrPtr(3);
                                lexer->updateLookAheadPtr();
                                lexer->foundToken();
                                break;
                            }
                        }

                        // '=/=' didn't happen
                        lexer->currentToken = new Token(lexer->lineNumber, token_type, buffer.substr(lexer->lookAheadPtr, 1), lexer->currPtr, lexer->buffer, buffer.substr(lexer->lookAheadPtr, 1).size(), lexer->fileName);
                        lexer->updateCurrPtr(1);
                        lexer->updateLookAheadPtr();
                        lexer->foundToken();
                        break;
                    }

                    case COMMENT: {
                        lexer->lexerState = COMMENT_FOUND;
                        lexer->currPtr = lexer->bufferLen;
                        break;
                    }

                    case COLON: {  // ':' => Maybe '::' or ':=' or Syntax Error

                        // Check for '::' or ':='
                        if ((lexer->lookAheadPtr + 1) < lexer->bufferLen) {
                            TOKEN_TYPE new_token_type = getTokenTypeUsingTwoChar(buffer[lexer->lookAheadPtr], buffer[lexer->lookAheadPtr + 1]);
                            if (new_token_type == ASSIGNMENT || new_token_type == BINDING) {
                                lexer->currentToken = new Token(lexer->lineNumber, new_token_type, buffer.substr(lexer->lookAheadPtr, 2), lexer->currPtr, lexer->buffer, buffer.substr(lexer->lookAheadPtr, 2).size(), lexer->fileName);
                                lexer->updateCurrPtr(2);
                                lexer->updateLookAheadPtr();
                                lexer->foundToken();
                                break;
                            }
                        }
                    }

                    case ERROR: {
                        lexer->currentLexerError = new LexerError(lexer->lineNumber, lexer->currPtr, STRAY_CHARACTER, lexer->buffer, 1);
                        lexer->errorOccured();
                        lexer->updateCurrPtr(1);
                        lexer->updateLookAheadPtr();
                        break;
                    }

                    case SPACE:
                    case TAB: {
                        lexer->updateCurrPtr(1);
                        lexer->updateLookAheadPtr();
                        break;
                    }

                    default: {  // {, }, (, ), [, ], ;, \n
                        lexer->currentToken = new Token(lexer->lineNumber, token_type, buffer.substr(lexer->lookAheadPtr, 1), lexer->currPtr, lexer->buffer, buffer.substr(lexer->lookAheadPtr, 1).size(), lexer->fileName);
                        lexer->updateCurrPtr(1);
                        lexer->updateLookAheadPtr();
                        lexer->foundToken();
                        break;
                    }
                }
            }  // Don't write code after this in this block
        } else if (lexer->currentLiteralState == STRING_LITERAL) {
            // Buf: "Hello world\uBye"
            // LAP:                  ^
            // Cur: ^
            if (lexer->stringState == STRING_STATE_ONE) {
                if (lexer->lookAheadPtr >= lexer->bufferLen) {
                    // NCBD => Error
                    lexer->currentLexerError = new LexerError(lexer->lineNumber, lexer->currPtr, INVALID_STRING, lexer->buffer, (lexer->lookAheadPtr - lexer->currPtr));
                    lexer->updateCurrPtr(0);
                    lexer->updateLookAheadPtr();
                    lexer->errorOccured();
                    lexer->currentLiteralState = NO_LITERAL;
                    lexer->stringState = STRING_STATE_START;
                } else if (buffer[lexer->lookAheadPtr] == '\\') {
                    lexer->stringState = STRING_STATE_TWO;
                    lexer->identifierTokenBuffer.push_back(buffer[lexer->lookAheadPtr]);
                    ++lexer->lookAheadPtr;
                } else if (buffer[lexer->lookAheadPtr] == '\"') {
                    lexer->stringState = STRING_STATE_END;
                    lexer->identifierTokenBuffer.push_back(buffer[lexer->lookAheadPtr]);
                    ++lexer->lookAheadPtr;
                } else {
                    lexer->identifierTokenBuffer.push_back(buffer[lexer->lookAheadPtr]);
                    ++lexer->lookAheadPtr;
                }
            } else if (lexer->stringState == STRING_STATE_TWO) {
                if (buffer[lexer->lookAheadPtr] == '\'' || buffer[lexer->lookAheadPtr] == '\"' || buffer[lexer->lookAheadPtr] == '\\' || buffer[lexer->lookAheadPtr] == 'n' || buffer[lexer->lookAheadPtr] == 'r' || buffer[lexer->lookAheadPtr] == 't' || buffer[lexer->lookAheadPtr] == 'b' || buffer[lexer->lookAheadPtr] == 'f' || buffer[lexer->lookAheadPtr] == 'v' || buffer[lexer->lookAheadPtr] == '0') {
                    lexer->stringState = STRING_STATE_ONE;
                    lexer->identifierTokenBuffer.push_back(buffer[lexer->lookAheadPtr]);
                    ++lexer->lookAheadPtr;
                } else {
                    // NCBD => Error
                    lexer->currentLexerError = new LexerError(lexer->lineNumber, lexer->currPtr, INVALID_STRING, lexer->buffer, (lexer->lookAheadPtr - lexer->currPtr + 1));
                    ++lexer->lookAheadPtr;
                    lexer->stringState = STRING_STATE_ONE;
                }
            } else if (lexer->stringState == STRING_STATE_END) {
                if ((lexer->currentLexerError != NULL) && (lexer->currentLexerError->errorType == INVALID_STRING)) {
                    lexer->currentLexerError->errorLength = (lexer->lookAheadPtr - lexer->currPtr);
                    lexer->currentLexerError->buffer = lexer->buffer;
                    lexer->updateCurrPtr(1);
                    lexer->updateLookAheadPtr();
                    lexer->errorOccured();
                    lexer->currentLiteralState = NO_LITERAL;
                    lexer->stringState = STRING_STATE_START;
                } else if (lexer->lookAheadPtr - lexer->currPtr > 0) {
                    lexer->currentToken = new Token(lexer->lineNumber, STRING_TOKEN, string(lexer->identifierTokenBuffer), lexer->currPtr, lexer->buffer, lexer->identifierTokenBuffer.size(), lexer->fileName);
                    lexer->updateCurrPtr(0);
                    lexer->updateLookAheadPtr();
                    lexer->foundToken();
                    lexer->currentLiteralState = NO_LITERAL;
                    lexer->stringState = STRING_STATE_START;
                }
            }
        } else if (lexer->currentLiteralState == VARIABLE_LITERAL) {
            bool isValid = isVariableLiteral(lexer->identifierTokenBuffer);
            if (isValid) {
                if (isKeyword(lexer->identifierTokenBuffer)) {
                    lexer->currentToken = new Token(lexer->lineNumber, KEYWORD, string(lexer->identifierTokenBuffer), lexer->currPtr, lexer->buffer, lexer->identifierTokenBuffer.size(), lexer->fileName);
                } else {
                    lexer->currentToken = new Token(lexer->lineNumber, VAR_IDENTIFIER, string(lexer->identifierTokenBuffer), lexer->currPtr, lexer->buffer, lexer->identifierTokenBuffer.size(), lexer->fileName);
                }
                lexer->foundToken();
            } else {
                lexer->currentLexerError = new LexerError(lexer->lineNumber, lexer->currPtr, INVALID_VAR_LITERAL, lexer->buffer, lexer->identifierTokenBuffer.size());
                lexer->errorOccured();
            }
            lexer->identifierTokenBuffer.clear();
            lexer->currentLiteralState = NO_LITERAL;
            lexer->updateCurrPtr(0);
        } else if (lexer->currentLiteralState == FUNCTION_LITERAL) {
            bool isValid = isFunctionLiteral(lexer->identifierTokenBuffer);
            if (isValid) {
                if (isKeyword(lexer->identifierTokenBuffer)) {
                    lexer->currentToken = new Token(lexer->lineNumber, KEYWORD, string(lexer->identifierTokenBuffer), lexer->currPtr, lexer->buffer, lexer->identifierTokenBuffer.size(), lexer->fileName);
                } else {
                    lexer->currentToken = new Token(lexer->lineNumber, FUNC_IDENTIFIER, string(lexer->identifierTokenBuffer), lexer->currPtr, lexer->buffer, lexer->identifierTokenBuffer.size(), lexer->fileName);
                }
                lexer->foundToken();
            } else {
                lexer->currentLexerError = new LexerError(lexer->lineNumber, lexer->currPtr, INVALID_FUNC_LITERAL, lexer->buffer, lexer->identifierTokenBuffer.size());
                lexer->errorOccured();
            }
            lexer->identifierTokenBuffer.clear();
            lexer->currentLiteralState = NO_LITERAL;
            lexer->updateCurrPtr(0);
        } else if (lexer->currentLiteralState == INTEGER_LITERAL) {
            bool isValid = isIntegerLiteral(lexer, lexer->identifierTokenBuffer);
            if (isValid) {
                lexer->currentToken = new Token(lexer->lineNumber, INTEGER_TOKEN, string(lexer->identifierTokenBuffer), lexer->currPtr, lexer->buffer, lexer->identifierTokenBuffer.size(), lexer->fileName);
                lexer->foundToken();
            } else {
                lexer->currentLexerError = new LexerError(lexer->lineNumber, lexer->currPtr, INVALID_INTEGER, lexer->buffer, lexer->identifierTokenBuffer.size());
                lexer->errorOccured();
            }
            lexer->identifierTokenBuffer.clear();
            lexer->currentLiteralState = NO_LITERAL;
            lexer->updateCurrPtr(0);
        } else if (lexer->currentLiteralState == FLOAT_LITERAL) {
            bool isValid = isFloatLiteral(lexer, lexer->identifierTokenBuffer);
            if (isValid) {
                lexer->currentToken = new Token(lexer->lineNumber, FLOAT_TOKEN, string(lexer->identifierTokenBuffer), lexer->currPtr, lexer->buffer, lexer->identifierTokenBuffer.size(), lexer->fileName);
                lexer->foundToken();
            } else {
                lexer->currentLexerError = new LexerError(lexer->lineNumber, lexer->currPtr, INVALID_FLOAT, lexer->buffer, lexer->identifierTokenBuffer.size());
                lexer->errorOccured();
            }
            lexer->identifierTokenBuffer.clear();
            lexer->currentLiteralState = NO_LITERAL;
            lexer->updateCurrPtr(0);
        }
        // NCBD AF
        // } else {
        //     bool isValid = isValidLiteral(lexer);
        //     if (isValid) {
        //         lexer->currentToken = getAppropriateToken(lexer);
        //         lexer->foundToken();
        //     } else {
        //         LEXER_ERROR_TYPE errorType;
        //         if (lexer->currentLiteralState == VARIABLE_LITERAL) {
        //             errorType = INVALID_VAR_LITERAL;
        //         } else if (lexer->currentLiteralState == FUNCTION_LITERAL) {
        //             errorType = INVALID_FUNC_LITERAL;
        //         } else if (lexer->currentLiteralState == INTEGER_LITERAL) {
        //             errorType = INVALID_INTEGER;
        //         } else if (lexer->currentLiteralState == FLOAT_LITERAL) {
        //             errorType = INVALID_FLOAT;
        //         }
        //         lexer->currentLexerError = new LexerError(lexer->lineNumber, lexer->currPtr, errorType, lexer->buffer, lexer->identifierTokenBuffer.size());
        //         lexer->errorOccured();
        //     }
        //     lexer->identifierTokenBuffer.clear();
        //     lexer->currentLiteralState = NO_LITERAL;
        //     lexer->updateCurrPtr(0);
        // }
    }
}

/*	function: getNextToken()
	Each statement in out language cannot transcend beyond a single
	Hence, we load single line buffers.

	Even block statements have the start and end on separate lines

	Comments can anywhere. Just neglect entire buffer once you read a single '#'
*/
void getNextToken(Lexer* lexer) {
    while (lexer->lexerState == LEXER_STATE::SEARCHING) {
        bool reloadBuffer = !(lexer->currPtr < lexer->bufferLen);
        if (reloadBuffer) {
            lexer->reloadBuffer();
            if (lexer->reachedEOF) {
                if (lexer->bufferLen == 0) {
                    return;
                }
                reloadBuffer = false;
            }
        }
        _getNextTokenHelper(lexer);
    }
    if (lexer->lexerState == COMMENT_FOUND) {
        if (lexer->reachedEOF) {
            return;
        } else {
            lexer->lexerState = SEARCHING;
        }
    }
}

Lexer* initLexer(string file_name) {
    Lexer* lexer = NULL;
    try {
        lexer = new Lexer(file_name);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return lexer;
    }
    return lexer;
}