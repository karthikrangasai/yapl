#include "parser.hpp"

/*	function: runParser
	param: parser

	Takes as input a parser object and accumalates all the tokens, errors.
*/
void runParser(Parser* parser) {
    do {
        getNextToken(parser->lexer);
        if (parser->lexer->lexerState == FOUND_TOKEN) {
            Token* token = new Token(*(parser->lexer->currentToken));
            parser->tokenList.push_back(token);
            parser->lexer->resetGotToken();
        } else if (parser->lexer->lexerState == ERROR_OCCURED) {
            LexerError* lexerError = new LexerError(*(parser->lexer->currentLexerError));
            parser->lexerErrors.push_back(lexerError);
            parser->lexer->reseterrorOccured();
        }
    } while (!(parser->lexer->reachedEOF && ((parser->lexer->currPtr >= parser->lexer->bufferLen))));
}

/*	function: initializeParser
	param: filename

	Takes as input a string type object and returns the parser object.
*/
Parser* initializeParser(string filename) {
    return new Parser(filename);
}