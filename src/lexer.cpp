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

// Test RE [a-zA-Z][_a-zA-Z0-9]*
bool isVariableLiteral(string identifier) {
    unsigned int bufferPointer = 0;
    if (belongsToFirstIdentifierAlphabet(identifier[bufferPointer])) {
        ++bufferPointer;
        while (bufferPointer < identifier.size()) {
            if (!belongsToIdentifierAlphabet(identifier[bufferPointer])) {
                return false;
            }
            ++bufferPointer;
        }
        return true;
    }
    return false;
}

// Test RE @[a-zA-Z][_a-zA-Z0-9]*
bool isFunctionLiteral(string identifier) {
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
    int state = 0;
    for (int i = 0; i < (int)identifier.size(); i++) {
        if (i == (int)(identifier.size() - 1)) {
            if (identifier[i] == '.') {
                lexer->currentLiteralState = FLOAT_LITERAL;
                return true;
            }
        }
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
//AAAAAH
void floatREStartStateEvaluator(char character, int* state) {
    if (character == '0') {
        *state = 3;
    } else if (character >= '1' and character <= '9') {
        *state = 4;
    } else
        *state = -1;
    return;
}
void state_2(char character, int* state) {
    if (character >= '1' and character <= '9') {
        *state = 2;
    } else if (character == '0') {
        *state = 5;
    } else
        *state = -1;
}
void state_3(char character, int* state) {
    if (character == '.') {
        *state = 5;
    } else
        *state = -1;
}
void state_5(char character, int* state) {
    if (character == '0') {
        *state = 5;
    } else if (character >= '1' and character <= '9') {
        *state = 2;
    } else
        *state = -1;
}
void state_4(char character, int* state) {
    if (character >= '0' and character <= '9') {
        *state = 4;
    } else if (character == '.') {
        *state = 5;
    } else
        *state = -1;
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
            return new Token(lexer->lineNumber, INTEGER, string(lexer->identifierTokenBuffer));
        }
        case FLOAT_LITERAL: {
            return new Token(lexer->lineNumber, FLOAT, string(lexer->identifierTokenBuffer));
        }
        case NO_LITERAL: {
            lexer->lexerState = ERROR_OCCURED;
            break;
        }
    }
    return NULL;
}

/*	function: _getNextTokenHelper()
	We have to read each the buffer char by char
	Then for each char:
		if char belongs to [a-zA-Z]:
			then check str[currPtr, lookAheadPtr]
			Then find full identifer and return IDENTIFIER / KEYWORD token
		else:
			Find it's TOKEN_TYPE and act accordingly
	Don't forget to update the lookAheadPtr.
*/
void _getNextTokenHelper(Lexer* lexer) {
    string buffer = lexer->buffer;

    while (!lexer->gotToken) {
        // Not sure what we are going to see
        if (lexer->currentLiteralState == NO_LITERAL) {
            // Checks for RE: [a-zA-Z]
            if (belongsToFirstIdentifierAlphabet(buffer[lexer->lookAheadPtr])) {
                lexer->identifierTokenBuffer = buffer.substr(lexer->currPtr, 1);
                lexer->currentLiteralState = VARIABLE_LITERAL;
                ++lexer->lookAheadPtr;
            } else if (buffer[lexer->lookAheadPtr] == '0') {
                // lexer->identifierTokenBuffer = buffer.substr(lexer->currPtr, 1);
                // lexer->currentLiteralState = INTEGER_LITERAL;
                // ++lexer->lookAheadPtr;
                /*	Error:
						01 => INTEGER INTEGER
						00.0 => INTEGER FLOAT
						0.00 => FLOAT INTEGER
				*/
                if (buffer[lexer->lookAheadPtr + 1] == '.') {
                    lexer->identifierTokenBuffer = buffer.substr(lexer->currPtr, 2);  // "0."
                    lexer->currentLiteralState = FLOAT_LITERAL;
                    ++(++lexer->lookAheadPtr);
                    // if (buffer[lexer->lookAheadPtr + 2] == '0') {
                    //     // you have reached 0.0
                    //     lexer->foundToken();
                    //     lexer->currentToken = new Token(lexer->lineNumber, FLOAT, "0.0");
                    //     lexer->updateCurrPtr(1);
                    //     lexer->updateLookAheadPtr();
                    // } else if (isNonZeroInteger(buffer[lexer->lookAheadPtr + 2])) {
                    //     /*
                    // 		you have reached 0.(1-9)
                    // 		Not error state, but 0.[1-9]*[0-9] => FLOAT_LITERAL
                    // 	*/
                    //     lexer->identifierTokenBuffer = buffer.substr(lexer->currPtr, 1);
                    //     lexer->currentLiteralState = FLOAT_LITERAL;
                    //     ++lexer->lookAheadPtr;
                    // }
                } else if (isNonZeroInteger(buffer[lexer->lookAheadPtr + 1])) {
                    // if non zero number => error
                    // you have reached 0(1-9)
                    // throw error
                    // 02345
                    // ^
                    //  ^
                    lexer->identifierTokenBuffer = buffer.substr(lexer->currPtr, 2);  // "0(1-9)"
                    ++(++lexer->lookAheadPtr);
                    while (isNonZeroInteger(buffer[lexer->lookAheadPtr])) {
                        lexer->identifierTokenBuffer.push_back(buffer[lexer->lookAheadPtr]);
                        ++lexer->lookAheadPtr;
                    }
                    lexer->lexerState = ERROR_OCCURED;
                    lexer->updateCurrPtr(0);
                    lexer->updateLookAheadPtr();
                    return;
                } else {
                    // return '0' as INTEGER
                    lexer->foundToken();
                    lexer->currentToken = new Token(lexer->lineNumber, INTEGER, "0");
                    lexer->updateCurrPtr(1);
                    lexer->updateLookAheadPtr();
                }
                // if character means parser will take care
            } else if (isNonZeroInteger(buffer[lexer->lookAheadPtr])) {
                // how to change between integer and float ??????????????
                /*	Brainstorming time
						123 => int
						123.4 => float

						How to predict ?????????????????????????????????????????????????????????????????

						123 => state = INTEGER_LITERAL
						if (state == INTEGER_LITERAL and currentChar == '.') => FLOAT LITERAL
				*/
                lexer->identifierTokenBuffer = buffer.substr(lexer->currPtr, 1);
                lexer->currentLiteralState = INTEGER_LITERAL;
                ++lexer->lookAheadPtr;
            } else {
                TOKEN_TYPE token_type = getTokenTypeUsingOneChar(buffer[lexer->lookAheadPtr]);

                switch (token_type) {
                    case AT_THE_RATE: {
                        if (belongsToFirstIdentifierAlphabet(buffer[lexer->lookAheadPtr + 1])) {
                            lexer->currentLiteralState = FUNCTION_LITERAL;
                            lexer->identifierTokenBuffer.push_back(buffer[lexer->lookAheadPtr]);  // Appends '@'
                            lexer->updateCurrPtr(1);
                            lexer->updateLookAheadPtr();
                        } else {
                            cout << "At char: " << buffer[lexer->lookAheadPtr] << " Error detected\n";
                            lexer->updateCurrPtr(1);
                            lexer->updateLookAheadPtr();
                            lexer->errorOccured();
                        }

                        break;
                    }

                    case LESS: {  // '<' => Maybe '<<' or '<=' or just simply '<'

                        // Check for '<<' or '<='
                        TOKEN_TYPE new_token_type = getTokenTypeUsingTwoChar(buffer[lexer->lookAheadPtr], buffer[lexer->lookAheadPtr + 1]);
                        if (new_token_type == LEFT_SHIFT || new_token_type == LESS_OR_EQUAL) {
                            lexer->currentToken = new Token(lexer->lineNumber, new_token_type, buffer.substr(lexer->lookAheadPtr, 2));
                            lexer->updateCurrPtr(2);
                            lexer->updateLookAheadPtr();
                            lexer->foundToken();
                            break;  // Break from swtich
                        }

                        // '<' only
                        lexer->currentToken = new Token(lexer->lineNumber, token_type, buffer.substr(lexer->lookAheadPtr, 1));
                        lexer->updateCurrPtr(1);
                        lexer->updateLookAheadPtr();
                        lexer->foundToken();
                        break;
                    }

                    case GREATER: {  // '>' => Maybe '>>' or '>=' or just simply '>'

                        // Check for '>>' or '>='
                        TOKEN_TYPE new_token_type = getTokenTypeUsingTwoChar(buffer[lexer->lookAheadPtr], buffer[lexer->lookAheadPtr + 1]);
                        if (new_token_type == RIGHT_SHIFT || new_token_type == GREATER_OR_EQUAL) {
                            lexer->currentToken = new Token(lexer->lineNumber, new_token_type, buffer.substr(lexer->lookAheadPtr, 2));
                            lexer->updateCurrPtr(2);
                            lexer->updateLookAheadPtr();
                            lexer->foundToken();
                            break;
                        }

                        // '>' only
                        lexer->currentToken = new Token(lexer->lineNumber, token_type, buffer.substr(lexer->lookAheadPtr, 1));
                        lexer->updateCurrPtr(1);
                        lexer->updateLookAheadPtr();
                        lexer->foundToken();
                        break;
                    }

                    case COLON: {  // ':' => Maybe '::' or ':=' or Syntax Error

                        // Check for '::' or ':='
                        TOKEN_TYPE new_token_type = getTokenTypeUsingTwoChar(buffer[lexer->lookAheadPtr], buffer[lexer->lookAheadPtr + 1]);
                        if (new_token_type == ASSIGNMENT || new_token_type == BINDING) {
                            string s;
                            s.push_back(buffer[lexer->lookAheadPtr]);
                            s.push_back(buffer[lexer->lookAheadPtr + 1]);
                            lexer->currentToken = new Token(lexer->lineNumber, new_token_type, s);
                            lexer->updateCurrPtr(2);
                            lexer->updateLookAheadPtr();
                            lexer->foundToken();
                            break;
                        }

                        // ':' only -> Error
                        cout << "At char " << buffer[lexer->lookAheadPtr] << " Syntax Error\n";
                        lexer->updateCurrPtr(1);
                        lexer->updateLookAheadPtr();
                        break;
                    }

                    case ADD: {  // ':' => Maybe '++'

                        // Check for '++'
                        TOKEN_TYPE new_token_type = getTokenTypeUsingTwoChar(buffer[lexer->lookAheadPtr], buffer[lexer->lookAheadPtr + 1]);
                        if (new_token_type == PRE_INCEREMENT) {
                            string s;
                            s.push_back(buffer[lexer->lookAheadPtr]);
                            s.push_back(buffer[lexer->lookAheadPtr + 1]);
                            lexer->currentToken = new Token(lexer->lineNumber, new_token_type, s);
                            lexer->updateCurrPtr(2);
                            lexer->updateLookAheadPtr();
                            lexer->foundToken();
                            break;
                        }

                        // '+' only
                        lexer->currentToken = new Token(lexer->lineNumber, token_type, buffer.substr(lexer->lookAheadPtr, 1));
                        lexer->updateCurrPtr(1);
                        lexer->updateLookAheadPtr();
                        lexer->foundToken();
                        break;
                    }

                    case SUBTRACT: {  // '-' => Maybe '--'

                        // Check for '--'
                        TOKEN_TYPE new_token_type = getTokenTypeUsingTwoChar(buffer[lexer->lookAheadPtr], buffer[lexer->lookAheadPtr + 1]);
                        if (new_token_type == PRE_DECREMENT) {
                            string s;
                            s.push_back(buffer[lexer->lookAheadPtr]);
                            s.push_back(buffer[lexer->lookAheadPtr + 1]);
                            lexer->currentToken = new Token(lexer->lineNumber, new_token_type, s);
                            lexer->updateCurrPtr(2);
                            lexer->updateLookAheadPtr();
                            lexer->foundToken();
                            break;
                        }

                        // '-' only
                        lexer->currentToken = new Token(lexer->lineNumber, token_type, buffer.substr(lexer->lookAheadPtr, 1));
                        lexer->updateCurrPtr(1);
                        lexer->updateLookAheadPtr();
                        lexer->foundToken();
                        break;
                    }

                    case EQUAL: {  // '=' => Maybe '=/=' or just simply '='

                        // Check for '=/=' TOKEN
                        TOKEN_TYPE new_token_type = getTokenTypeUsingThreeChar(buffer[lexer->lookAheadPtr], buffer[lexer->lookAheadPtr + 1], buffer[lexer->lookAheadPtr + 2]);
                        if (new_token_type == NOTEQUAL) {
                            lexer->currentToken = new Token(lexer->lineNumber, new_token_type, buffer.substr(lexer->lookAheadPtr, 3));
                            lexer->updateCurrPtr(3);
                            lexer->updateLookAheadPtr();
                            lexer->foundToken();
                            break;
                        }

                        // '=/=' didn't happen
                        lexer->currentToken = new Token(lexer->lineNumber, token_type, buffer.substr(lexer->lookAheadPtr, 1));
                        lexer->updateCurrPtr(1);
                        lexer->updateLookAheadPtr();
                        lexer->foundToken();
                        break;
                    }

                    case COMMENT: {
                        // cout << "Comment detected\n";
                        lexer->lexerState = COMMENT_FOUND;
                        lexer->currPtr = lexer->bufferLen;
                        return;
                    }

                    case ERROR: {
                        cout << "At char: " << buffer[lexer->lookAheadPtr] << " Error detected\n";
                        lexer->updateCurrPtr(1);
                        lexer->updateLookAheadPtr();
                        lexer->errorOccured();
                        break;
                    }

                    default: {  // {, }, (, ), [, ], ;, \n, " ", \t
                        lexer->currentToken = new Token(lexer->lineNumber, token_type, buffer.substr(lexer->lookAheadPtr, 1));
                        lexer->updateCurrPtr(1);
                        lexer->updateLookAheadPtr();
                        lexer->foundToken();
                        break;
                    }
                }
            }  // Don't write code after this in this block
        } else {
            lexer->identifierTokenBuffer.push_back(buffer[lexer->lookAheadPtr]);
            bool isValid = isValidLiteral(lexer);
            if (isValid) {
                ++lexer->lookAheadPtr;
            } else {
                if (lexer->lexerState == ERROR_OCCURED) {
                    return;
                }
                lexer->identifierTokenBuffer.pop_back();
                lexer->currentToken = getAppropriateToken(lexer);
                lexer->foundToken();
                lexer->identifierTokenBuffer.clear();
                lexer->updateCurrPtr(0);
                lexer->currentLiteralState = NO_LITERAL;
            }
        }
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
        bool reloadBuffer = !((lexer->currPtr != -1) && (lexer->currPtr < lexer->bufferLen));
        if (reloadBuffer) {
            lexer->reloadBuffer();
            bool reachedEOF = lexer->source_file.eof();
            if (reachedEOF) {
                lexer->reachedEOF = true;
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