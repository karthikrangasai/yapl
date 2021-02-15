// Receive file from main.cpp and send it to lexer.cpp
#include "parser.hpp"

#include <stdlib.h>
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
        cout << "Token Type: " << token->token_name << ", ";
        cout << "Lexeme: " << token->lexeme << "\n";
    }
}

void runParser(Parser* parser) {
    // string input;
    // cout << "Please enter something: ";
    // cin >> input;
    // while (input != "quit") {
    //     getNextToken(parser->lexer);
    //     if (parser->lexer->lexerState == FOUND_TOKEN) {
    //         Token* token = new Token(*(parser->lexer->currentToken));
    //         parser->tokenList.push_back(token);
    //         printTokenMessage(token);
    //         parser->lexer->resetGotToken();
    //     } else if (parser->lexer->lexerState == ERROR_OCCURED) {
    //         cout << "Syntax Error at line " << parser->lexer->lineNumber << ".\n";
    //         // parser->lexer->lexerState = SEARCHING;
    //         parser->lexer->reseterrorOccured();
    //     }
    //     cin >> input;
    // }

    // While lexer is searching, keep running
    // Once ERROR_OCCURED or TOKEN_FOUND
    // Then take action

    do {
        getNextToken(parser->lexer);  // }
        if (parser->lexer->lexerState == FOUND_TOKEN) {
            Token* token = new Token(*(parser->lexer->currentToken));
            parser->tokenList.push_back(token);
            // printTokenMessage(token);
            parser->lexer->resetGotToken();
        } else if (parser->lexer->lexerState == ERROR_OCCURED) {
            LexerError* lexerError = new LexerError(*(parser->lexer->currentLexerError));
            parser->lexerErrors.push_back(lexerError);
            // cout << parser->lexer->fileName << ":";
            // parser->lexer->currentLexerError->print(parser->lexer->fileName);
            // cout << "";
            // cout << parser->lexer->fileName << ":" << parser->lexer->lineNumber << ":";
            // cout << "Syntax Error at line " << parser->lexer->lineNumber << ".\n";
            parser->lexer->reseterrorOccured();
        }
    } while (!(parser->lexer->reachedEOF && ((parser->lexer->currPtr >= parser->lexer->bufferLen))));

    // string input;
    // do {
    //     cin >> input;
    //     getNextToken(parser->lexer);  // }
    //     if (parser->lexer->lexerState == FOUND_TOKEN) {
    //         Token* token = new Token(*(parser->lexer->currentToken));
    //         parser->tokenList.push_back(token);
    //         printTokenMessage(token);
    //         parser->lexer->resetGotToken();
    //     } else if (parser->lexer->lexerState == ERROR_OCCURED) {
    //         cout << "Syntax Error at line " << parser->lexer->lineNumber << ".\n";
    //         parser->lexer->reseterrorOccured();
    //     }
    // } while (input != "quit");
}

Parser* initializeParser(string filename) {
    return new Parser(filename);
}