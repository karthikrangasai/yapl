// Receive file from main.cpp and send it to lexer.cpp
#include "parser.hpp"
// Open file
// Request token as and when necessary
// Print (Line Number: %d, token_type, token_code, lexeme)
//		// generate BS
// Close file

// 	   int line_number;
//     TOKEN_TYPE token;
//     const char* lexeme;

void printTokenMessage(Token* token) {
    if (token != NULL) {
        cout << "Line Number: " << token->line_number << ", ";
        cout << "Token Code: " << token->token << ", ";
        cout << "Token Type: " << getTokenName(token->token) << ", ";
        cout << "Lexeme: " << token->lexeme << "\n";
    }
}

bool startParser(string filename) {
    // Initialize Lexer Struct
    bool success = initLexer(filename);  // lexer.c
    if (!success) {
        return success;
    }

    // While getToken() doesn't give NULL/EOF keep going Current print

    // string input;
    // cout << "Please enter something: ";
    // cin >> input;
    // while (input != "quit") {
    //     Token* next_token = getNextToken();
    //     if (next_token != NULL) {
    //         printTokenMessage(next_token);
    //         cin >> input;
    //     } else if (lexer_state->eof) {
    //         break;
    //     }
    // }

    Token* next_token = getNextToken();
    while (true) {
        if (next_token == NULL || lexer_state->eof) {
            if (!(lexer_state->emptyBuffer)) {
                break;
            }
        }
        printTokenMessage(next_token);
        next_token = getNextToken();
    }

    return true;
}