#include "lexer.hpp"

#include <iostream>
#include <sstream>
#include <string>
using namespace std;

// Global Variables
lexer* lexer_state;

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
    int bufferPointer = 0;
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

bool isValidLiteral(string identifier) {
    switch (lexer_state->currentLiteralState) {
        case VARIABLE_LITERAL: {
            return isVariableLiteral(identifier);
        }
        case FUNCTION_LITERAL: {
            return isFunctionLiteral(identifier);
        }
            // case INTEGER_LITERAL: {
            //     return isVariableLiteral(identifier);
            // }
            // case FLOAT_LITERAL: {
            //     return isVariableLiteral(identifier);
            // }
    }
    return false;
}

Token* getAppropriateToken() {
    switch (lexer_state->currentLiteralState) {
        case VARIABLE_LITERAL:
        case FUNCTION_LITERAL: {
            if (isKeyword(lexer_state->identifierTokenBuffer)) {
                return new Token(lexer_state->lineNumber, KEYWORD, string(lexer_state->identifierTokenBuffer));
            }
            return new Token(
                lexer_state->lineNumber,
                (lexer_state->currentLiteralState == VARIABLE_LITERAL) ? VAR_IDENTIFIER : FUNC_IDENTIFIER,
                string(lexer_state->identifierTokenBuffer));
        }
            // case INTEGER_LITERAL: {
            //     return isVariableLiteral(identifier);
            // }
            // case FLOAT_LITERAL: {
            //     return isVariableLiteral(identifier);
            // }
    }
    return NULL;
}

/*	method: _getNextTokenHelper()
	We have to read each the buffer char by char
	Then for each char:
		if char belongs to [a-zA-Z]:
			then check str[currPtr, lookAheadPtr]
			Then find full identifer and return IDENTIFIER / KEYWORD token
		else:
			Find it's TOKEN_TYPE and act accordingly
	Don't forget to update the lookAheadPtr.
*/
Token* _getNextTokenHelper() {
    string buffer = lexer_state->buffer;
    Token* token = NULL;

    while (!lexer_state->gotToken) {
        // char currChar = buffer[lexer_state->lookAheadPtr];
        // Not sure what we are going to see
        if (lexer_state->currentLiteralState == NO_LITERAL) {
            // Checks for RE: [a-zA-Z]
            if (belongsToFirstIdentifierAlphabet(buffer[lexer_state->lookAheadPtr])) {
                lexer_state->identifierTokenBuffer = buffer.substr(lexer_state->currPtr, 1);
                lexer_state->currentLiteralState = VARIABLE_LITERAL;
                ++lexer_state->lookAheadPtr;
            } else if (false) {
                // Add integer identifier
                // See (0 | [1-9]) => Cut to number literal

                // Float identifier = integer and you see a dot : I think
            } else {
                TOKEN_TYPE token_type = getTokenTypeUsingOneChar(buffer[lexer_state->lookAheadPtr]);

                switch (token_type) {
                    case AT_THE_RATE: {
                        lexer_state->currentLiteralState = FUNCTION_LITERAL;
                        lexer_state->identifierTokenBuffer.push_back(buffer[lexer_state->lookAheadPtr]);  // Appends '@'
                        lexer_state->updateCurrPtr(1);
                        lexer_state->updateLookAheadPtr();
                        break;
                    }

                    case LESS: {  // '<' => Maybe '<<' or '<=' or just simply '<'

                        // Check for '<<' or '<='
                        TOKEN_TYPE new_token_type = getTokenTypeUsingTwoChar(buffer[lexer_state->lookAheadPtr], buffer[lexer_state->lookAheadPtr + 1]);
                        if (new_token_type == LEFT_SHIFT || new_token_type == LESS_OR_EQUAL) {
                            token = new Token(lexer_state->lineNumber, new_token_type, buffer.substr(lexer_state->lookAheadPtr, 2));
                            lexer_state->updateCurrPtr(2);
                            lexer_state->updateLookAheadPtr();
                            lexer_state->foundToken();
                            break;  // Break from swtich
                        }

                        // '<' only
                        token = new Token(lexer_state->lineNumber, token_type, buffer.substr(lexer_state->lookAheadPtr, 1));
                        lexer_state->updateCurrPtr(1);
                        lexer_state->updateLookAheadPtr();
                        lexer_state->foundToken();
                        break;
                    }

                    case GREATER: {  // '>' => Maybe '>>' or '>=' or just simply '>'

                        // Check for '>>' or '>='
                        TOKEN_TYPE new_token_type = getTokenTypeUsingTwoChar(buffer[lexer_state->lookAheadPtr], buffer[lexer_state->lookAheadPtr + 1]);
                        if (new_token_type == RIGHT_SHIFT || new_token_type == GREATER_OR_EQUAL) {
                            token = new Token(lexer_state->lineNumber, new_token_type, buffer.substr(lexer_state->lookAheadPtr, 2));
                            lexer_state->updateCurrPtr(2);
                            lexer_state->updateLookAheadPtr();
                            lexer_state->foundToken();
                            break;
                        }

                        // '>' only
                        token = new Token(lexer_state->lineNumber, token_type, buffer.substr(lexer_state->lookAheadPtr, 1));
                        lexer_state->updateCurrPtr(1);
                        lexer_state->updateLookAheadPtr();
                        lexer_state->foundToken();
                        break;
                    }

                    case COLON: {  // ':' => Maybe '::' or ':=' or Syntax Error

                        // Check for '::' or ':='
                        TOKEN_TYPE new_token_type = getTokenTypeUsingTwoChar(buffer[lexer_state->lookAheadPtr], buffer[lexer_state->lookAheadPtr + 1]);
                        if (new_token_type == ASSIGNMENT || new_token_type == BINDING) {
                            string s;
                            s.push_back(buffer[lexer_state->lookAheadPtr]);
                            s.push_back(buffer[lexer_state->lookAheadPtr + 1]);
                            token = new Token(lexer_state->lineNumber, new_token_type, s);
                            lexer_state->updateCurrPtr(2);
                            lexer_state->updateLookAheadPtr();
                            lexer_state->foundToken();
                            break;
                        }

                        // ':' only -> Error
                        cout << "At char " << buffer[lexer_state->lookAheadPtr] << " Syntax Error\n";
                        lexer_state->updateCurrPtr(1);
                        lexer_state->updateLookAheadPtr();
                        break;
                    }

                    case EQUAL: {  // '=' => Maybe '=/=' or just simply '='

                        // Check for '=/=' TOKEN
                        TOKEN_TYPE new_token_type = getTokenTypeUsingThreeChar(buffer[lexer_state->lookAheadPtr], buffer[lexer_state->lookAheadPtr + 1], buffer[lexer_state->lookAheadPtr + 2]);
                        if (new_token_type == NOTEQUAL) {
                            token = new Token(lexer_state->lineNumber, new_token_type, buffer.substr(lexer_state->lookAheadPtr, 3));
                            lexer_state->updateCurrPtr(3);
                            lexer_state->updateLookAheadPtr();
                            lexer_state->foundToken();
                            break;
                        }

                        // '=/=' didn't happen
                        token = new Token(lexer_state->lineNumber, token_type, buffer.substr(lexer_state->lookAheadPtr, 1));
                        lexer_state->updateCurrPtr(1);
                        lexer_state->updateLookAheadPtr();
                        lexer_state->foundToken();
                        break;
                    }

                    case COMMENT: {
                        // Check for comment
                        lexer_state->updateCurrPtr(lexer_state->bufferLen - lexer_state->lookAheadPtr);
                        lexer_state->updateLookAheadPtr();
                        lexer_state->foundToken();  // Small hackk to exit the loop
                        break;
                    }

                    case ERROR: {
                        cout << "At char: " << buffer[lexer_state->lookAheadPtr] << " Error detected\n";
                        lexer_state->updateCurrPtr(1);
                        lexer_state->updateLookAheadPtr();
                        return token;
                    }

                    default: {  // {, }, (, ), [, ], ;, \n, " ", \t
                        token = new Token(lexer_state->lineNumber, token_type, buffer.substr(lexer_state->lookAheadPtr, 1));
                        lexer_state->updateCurrPtr(1);
                        lexer_state->updateLookAheadPtr();
                        lexer_state->foundToken();
                        break;
                    }
                }
            }  // Don't write code after this in this block
        } else {
            lexer_state->identifierTokenBuffer.push_back(buffer[lexer_state->lookAheadPtr]);
            bool isValid = isValidLiteral(lexer_state->identifierTokenBuffer);
            if (isValid) {
                ++lexer_state->lookAheadPtr;
            } else {
                lexer_state->identifierTokenBuffer.pop_back();
                token = getAppropriateToken();
                lexer_state->foundToken();
                lexer_state->identifierTokenBuffer.clear();
                lexer_state->updateCurrPtr(0);
                lexer_state->currentLiteralState = NO_LITERAL;
            }
        }
    }
    return token;
}

/*	method: getNextToken()
	Each statement in out language cannot transcend beyond a single
	Hence, we load single line buffers.

	Even block statements have the start and end on separate lines

	Comments can anywhere. Just neglect entire buffer once you read a single '#'
*/
Token* getNextToken() {
    bool reloadBuffer = !((lexer_state->currPtr != -1) && (lexer_state->currPtr < lexer_state->bufferLen));
    if (reloadBuffer) {
        // Please check for EOF
        if (lexer_state->source_file.eof()) {
            reloadBuffer = false;
            lexer_state->eof = true;
            return NULL;
        } else {
            getline(lexer_state->source_file, lexer_state->buffer);
            // cout << "Current buffer: " << lexer_state->buffer << " with " << lexer_state->buffer.size() << " chars\n";
            lexer_state->emptyBuffer = (lexer_state->buffer.size() == 0);
            if (lexer_state->emptyBuffer) {
                return NULL;
            }
            ++lexer_state->lineNumber;
            lexer_state->currPtr = lexer_state->lookAheadPtr = -1;
            lexer_state->bufferLen = lexer_state->buffer.size();
            // Reset pointers
            lexer_state->currPtr = lexer_state->lookAheadPtr = 0;
            // Finsished loading new line from file
        }
    }
    lexer_state->resetGotToken();
    return _getNextTokenHelper();
}

bool initLexer(string file_name) {
    try {
        lexer_state = new lexer(file_name);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}