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
            // parser->lexer->currentLexerError->print(parser->lexer->fileName);
            parser->lexer->reseterrorOccured();
        }
    } while (!(parser->lexer->reachedEOF && ((parser->lexer->currPtr >= parser->lexer->bufferLen))));
}

Parser* initializeParser(string filename) {
    return new Parser(filename);
}