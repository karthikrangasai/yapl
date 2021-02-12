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
bool isVariableIdentifier(string identifier) {
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

/*	method: _getNextTokenHelper()
	We have to read each the buffer char by char
	Then for each char:
		if char belongs to [a-zA-Z]:
			then check str[currPtr, lookAheadPtr]
			Then find full identifer and return IDENTIFIER / KEYWORD token
		else:
			Find it's TOKEN_TYPE and act accordingly
	
	Don't forget to update the lookAheadPtr.


	Space Indentation Rules:
		After the keywords: if, else, for, while, function, @main, return
		After the tokens: 
			} : Always, coz } is followed by end or end_definition
			) : Not always, only when the keywords are if, for, while, function
				For expressions, not needed.
		What else ?
*/
//	Buf: function
//	Cur: ^
//	LAP: ^
Token* _getNextTokenHelper() {
    string buffer = lexer_state->buffer;
    Token* token = NULL;

    while (!lexer_state->gotToken) {
        // char currChar = buffer[lexer_state->lookAheadPtr];
        // Not sure what we are going to see
        if (lexer_state->identifierState == NON_IDENTIFIER_CHECK || lexer_state->funcIdentifierState == FUNC_IDENTIFIER_START) {
            // Checks for RE: [a-zA-Z]
            if (belongsToFirstIdentifierAlphabet(buffer[lexer_state->lookAheadPtr])) {
                lexer_state->identifierTokenBuffer = buffer.substr(lexer_state->currPtr, 1);
                lexer_state->identifierState = IDENTIFIER_CHECK;
                ++lexer_state->lookAheadPtr;
                if (lexer_state->funcIdentifierState == FUNC_IDENTIFIER_START) {
                    lexer_state->funcIdentifierState = FUNC_IDENTIFIER_CHECK;
                }
            } else {
                // Remaining Hungama
                TOKEN_TYPE token_type = getTokenTypeUsingOneChar(buffer[lexer_state->lookAheadPtr]);

                switch (token_type) {
                    case AT_THE_RATE: {
                        //    : @factorail
                        // Buf: @main
                        // Cur:  ^
                        // LAP:   ^
                        lexer_state->funcIdentifierState = FUNC_IDENTIFIER_START;
                        // lexer_state->identifierTokenBuffer.push_back(buffer[lexer_state->lookAheadPtr]);  // Appends '@'
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
                            // token = new Token();
                            // token->line_number = lexer_state->lineNumber;
                            // token->token = new_token_type;
                            // token->lexeme = string(s);
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
                        // Cut to next line and restart the loading process
                        cout << "At char: " << buffer[lexer_state->lookAheadPtr] << " Comment detected\n";
                        lexer_state->updateCurrPtr(1);
                        lexer_state->updateLookAheadPtr();
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
        } else if (lexer_state->identifierState == IDENTIFIER_CHECK || lexer_state->funcIdentifierState == FUNC_IDENTIFIER_CHECK) {
            // ***** Asset that both in if statement can't be true. If they are, then syntax error

            lexer_state->identifierTokenBuffer.push_back(buffer[lexer_state->lookAheadPtr]);
            if (isVariableIdentifier(lexer_state->identifierTokenBuffer)) {
                // If buffer[currPtr, lookAheadPtr] == [a-zA-Z][_a-zA-Z0-9]*
                // Keep updating to check next char
                ++lexer_state->lookAheadPtr;
            } else {
                // Once buffer[currPtr, lookAheadPtr] != [a-zA-Z][_a-zA-Z0-9]*
                // Retract lookAheadPtr
                // --lexer_state->lookAheadPtr;

                // Delete last char in `identifierTokenBuffer`
                lexer_state->identifierTokenBuffer.pop_back();

                // Now `identifierTokenBuffer` has a valid indentifer (valid based on RE)
                if (lexer_state->funcIdentifierState == FUNC_IDENTIFIER_CHECK) {
                    // --lexer_state->currPtr;
                    if (isKeyword("@" + lexer_state->identifierTokenBuffer)) {
                        token = new Token(lexer_state->lineNumber, KEYWORD, "@" + string(lexer_state->identifierTokenBuffer));
                    } else {
                        token = new Token(lexer_state->lineNumber, FUNC_INDENTIFIER, "@" + string(lexer_state->identifierTokenBuffer));
                    }
                    lexer_state->funcIdentifierState = NON_FUNC_IDENTIFIER_CHECK;
                } else {
                    if (isKeyword(lexer_state->identifierTokenBuffer)) {
                        token = new Token(lexer_state->lineNumber, KEYWORD, string(lexer_state->identifierTokenBuffer));
                    } else {
                        token = new Token(lexer_state->lineNumber, VAR_INDENTIFIER, string(lexer_state->identifierTokenBuffer));
                    }
                }
                lexer_state->identifierState = NON_IDENTIFIER_CHECK;
                lexer_state->foundToken();
                lexer_state->identifierTokenBuffer.clear();
                lexer_state->updateCurrPtr(0);
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

            cout << "Current buffer: " << lexer_state->buffer << " with " << lexer_state->buffer.size() << " chars\n";
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
    // Open the file here
    // cout << file_name << "\n";

    try {
        lexer_state = new lexer(file_name);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}