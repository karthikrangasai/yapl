#include "lexer.hpp"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// Global Variables
lexer* lexer_state;

void printError(Error error) {}
bool isVariableIdentifier(char* identifier) {
    // Implement the DFA: [a-zA-Z][_a-zA-Z0-9]*
    return true;
}
bool isFunctionIdentifier(char* identifier) {
    // return (identifier[0] == '@') && isVariableIdentifier(identifier[1]);
    return true;
}

Token* _getNextTokenHelper() {
    string buffer = lexer_state->buffer;
    cout << buffer << "\n";
    // lexer_state->currPtr = lexer_state->lookAheadPtr = 0;  // Reset pointers

    Token* token = NULL;

    while (!lexer_state->gotToken) {
        // ++lexer_state->lookAheadPtr;

        // Check [a-zA-Z] -> IDENTFIER
        char nextChar = buffer[lexer_state->lookAheadPtr];
        if (belongsToFirstIdentifierAlphabet(buffer[lexer_state->lookAheadPtr])) {
            // If you find a alphabet

            cout << "At char: " << buffer[lexer_state->lookAheadPtr] << " Identifier detected\n";
            lexer_state->updateCurrPtr(1);
            lexer_state->updateLookAheadPtr();
            cout << "returning token";
            return token;
        }

        TOKEN_TYPE token_type = getTokenTypeUsingOneChar(buffer[lexer_state->lookAheadPtr]);
        cout << "Character searched: " << buffer[lexer_state->lookAheadPtr] << "\n";
        // cout << "Received Token Type: " << getTokenName(token_type) << "\n";
        switch (token_type) {
            case AT_THE_RATE: {
                // Check for @ -> func identifier
                cout << "At char: " << buffer[lexer_state->lookAheadPtr] << " Function Identifier detected\n";
                lexer_state->updateCurrPtr(1);
                lexer_state->updateLookAheadPtr();
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

            case LESS: {
                // Check for '<', go to twoCharPtr

                // Check for '<<' or '<='
                TOKEN_TYPE new_token_type = getTokenTypeUsingTwoChar(buffer[lexer_state->lookAheadPtr], buffer[lexer_state->lookAheadPtr + 1]);
                if (new_token_type == LEFT_SHIFT || new_token_type == LESS_OR_EQUAL) {
                    token = new Token(lexer_state->lineNumber, new_token_type, "example_string");
                    lexer_state->updateCurrPtr(2);
                    lexer_state->updateLookAheadPtr();
                    lexer_state->foundToken();
                    break;  // Break from swtich
                }

                // '<' only
                token = new Token(lexer_state->lineNumber, token_type, "example_string");
                lexer_state->updateCurrPtr(1);
                lexer_state->updateLookAheadPtr();
                lexer_state->foundToken();
                break;
            }

            case GREATER: {
                // Check for '>', go to twoCharPtr

                // Check for '>>' or '>='
                TOKEN_TYPE new_token_type = getTokenTypeUsingTwoChar(buffer[lexer_state->lookAheadPtr], buffer[lexer_state->lookAheadPtr + 1]);
                if (new_token_type == RIGHT_SHIFT || new_token_type == GREATER_OR_EQUAL) {
                    token = new Token(lexer_state->lineNumber, new_token_type, "example_string");
                    lexer_state->updateCurrPtr(2);
                    lexer_state->updateLookAheadPtr();
                    lexer_state->foundToken();
                    break;
                }

                // '>' only
                token = new Token(lexer_state->lineNumber, token_type, "example_string");
                lexer_state->updateCurrPtr(1);
                lexer_state->updateLookAheadPtr();
                lexer_state->foundToken();
                break;
            }

            case COLON: {
                // Check for ':', go to twoCharPtr

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

            case EQUAL: {
                // Check for '=' and test threeCharPtr

                // Check for '=/=' TOKEN
                TOKEN_TYPE new_token_type = getTokenTypeUsingThreeChar(buffer[lexer_state->lookAheadPtr], buffer[lexer_state->lookAheadPtr + 1], buffer[lexer_state->lookAheadPtr + 2]);
                if (new_token_type == NOTEQUAL) {
                    token = new Token(lexer_state->lineNumber, new_token_type, "example_string");
                    lexer_state->updateCurrPtr(3);
                    lexer_state->updateLookAheadPtr();
                    lexer_state->foundToken();
                    break;
                }
                // '=/=' didn't happen
                token = new Token(lexer_state->lineNumber, token_type, "example_string");
                lexer_state->updateCurrPtr(1);
                lexer_state->updateLookAheadPtr();
                lexer_state->foundToken();
                break;
            }

            case ERROR: {
                cout << "At char: " << buffer[lexer_state->lookAheadPtr] << " Identifier detected\n";
                lexer_state->updateCurrPtr(1);
                lexer_state->updateLookAheadPtr();
                return token;
            }
            default: {  // {, }, (, ), [, ], ;, \n, " ", \t
                token = new Token(lexer_state->lineNumber, token_type, "example_string");
                lexer_state->updateCurrPtr(1);
                lexer_state->updateLookAheadPtr();
                lexer_state->foundToken();
                break;
            }
        }
    }
    return token;
}

Token* getNextToken() {
    // The very first token will always be `function`

    // Read line by line since no way code can transcend beyond one line
    // Return token objects
    // integer::a:=1;

    // We have to read each line char by char
    // Then for each char, check it is an operator, else update the
    // the lookAheadPtr, then check str[currPtr, lookAheadPtr]
    bool reloadBuffer = !((lexer_state->currPtr != -1) && (lexer_state->currPtr < lexer_state->bufferLen));
    if (reloadBuffer) {
        getline(lexer_state->source_file, lexer_state->buffer);
        ++lexer_state->lineNumber;
        lexer_state->currPtr = lexer_state->lookAheadPtr = -1;
        lexer_state->bufferLen = lexer_state->buffer.size();
        lexer_state->currPtr = lexer_state->lookAheadPtr = 0;  // Reset pointers
                                                               // Finsished loading new line from file
    }
    return _getNextTokenHelper();
}

bool initLexer(string filename) {
    // Open the file here
    cout << filename << "\n";

    try {
        lexer_state = new lexer(filename);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}