/**
 * @file parser.hpp
 * @brief 
 */
#ifndef PARSER_H
#define PARSER_H
#include <unordered_map>
#include <variant>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <stack>

#include "lexer.hpp"
using namespace std;

// #ifdef __cplusplus
// extern "C" {
// #endif

/**
 * @brief List of all the Terminals
 */
enum TERMINALS {
    NONE_TERMINAL = -1,
    EMPTY,
    IDENTIFIER,
    FUNCTION_IDENTIFIER,
    FUNTION_KEYWORD,
    MODULO_OPERATOR,
    DIVISION_OPERATOR,
    MULTIPLICATION_OPERATOR,
    ADDITION_OPERATOR,
    SUBTRACTION_OPERATOR,
    LEFT_PARENTHESES,
    RIGHT_PARENTHESES,
    LEFT_FLOWER_BRACKET,
    RIGHT_FLOWER_BRACKET,
    COMMA_SEPARATOR,
    SEMI_COLON_SEPARATOR,
    BINDING_OPERATOR,
    END_DEFINITION_KEYWORD,
    POSITIVE_INTEGER,
    TRUE_KEYWORD,
    FALSE_KEYWORD,
    BOOLEAN_KEYWORD,
    INTEGER_KEYWORD,
    VOID_KEYWORD,
    LESS_THAN_EQUAL_OPERATOR,
    GREATER_THAN_EQUAL_OPERATOR,
    LESS_THAN_OPERATOR,
    GREATER_THAN_OPERATOR,
    EQAULITY_OPERATOR,
    INEQUALITY_OPERATOR,
    ASSIGNMENT_OPERATOR,
    BREAK_KEYWORD,
    CONTINUE_KEYWORD,
    RETURN_KEYWORD,
    WHILE_KEYWORD,
    END_KEYWORD,
    IF_KEYWORD,
    ELSE_KEYWORD,
    END_MARKER,
};

const unsigned int numTerminals = 0;

/**
 * @brief O(1) lookup table for string to enum conversion of all the Terminals.
 */
const vector<string> terminal_names = {
    // "NONE_TERMINAL",
    "EMPTY",
    "IDENTIFIER",
    "FUNCTION_IDENTIFIER",
    "FUNTION_KEYWORD",
    "MODULO_OPERATOR",
    "DIVISION_OPERATOR",
    "MULTIPLICATION_OPERATOR",
    "ADDITION_OPERATOR",
    "SUBTRACTION_OPERATOR",
    "LEFT_PARENTHESES",
    "RIGHT_PARENTHESES",
    "LEFT_FLOWER_BRACKET",
    "RIGHT_FLOWER_BRACKET",
    "COMMA_SEPARATOR",
    "SEMI_COLON_SEPARATOR",
    "BINDING_OPERATOR",
    "END_DEFINITION_KEYWORD",
    "POSITIVE_INTEGER",
    "TRUE_KEYWORD",
    "FALSE_KEYWORD",
    "BOOLEAN_KEYWORD",
    "INTEGER_KEYWORD",
    "VOID_KEYWORD",
    "LESS_THAN_EQUAL_OPERATOR",
    "GREATER_THAN_EQUAL_OPERATOR",
    "LESS_THAN_OPERATOR",
    "GREATER_THAN_OPERATOR",
    "EQAULITY_OPERATOR",
    "INEQUALITY_OPERATOR",
    "ASSIGNMENT_OPERATOR",
    "BREAK_KEYWORD",
    "CONTINUE_KEYWORD",
    "RETURN_KEYWORD",
    "WHILE_KEYWORD",
    "END_KEYWORD",
    "IF_KEYWORD",
    "ELSE_KEYWORD",
    "END_MARKER",
};

// map<TERMINALS, string> terminalValueLookup;

/**
 * @brief List of all the Non-Terminals.
 */
enum NON_TERMINALS {
    NONE_NON_TERMINAL = -1,
    PROGRAM,
    FUNCTIONS,
    FUNCTION,
    FUNCTION_PARAMETERS,
    PARAMETERS,
    MORE_PARAMETERS,
    PARAMETER,
    COMPOUND_STATEMENTS,
    STATEMENT,
    LOOP_STATEMENT,
    CONDITIONAL_STATEMENTS,
    ELSE_STATEMENT,
    BREAK_STATEMENTS,
    RETURN_ENTITY,
    FUNCTION_CALL,
    ARGUEMENTS,
    ADDITIONAL_ARGUMENTS,
    INITIALIZATION_STATEMENTS,
    ASSIGN_STATEMENT,
    TERMIMAL_EXPRESSIONS,
    ADDITIONAL_TERMINALS,
    EXPRESSION,
    LOGICAL_EXPRESSION,
    RELATIONAL_EXPRESSION,
    RELATION_OPERATION,
    MORE_REL_OP,
    NUMERIC_EXPRESSION,
    NUMERIC_OPERATION,
    MULTIPLICATION_EXPRESSION,
    MULTIPLICATION_OPERATION,
    REL_OP,
    REL_EQ_OP,
    REL_INEQ_OP,
    TERMIMAL_EXPRESSION,
    TERMINAL_ID,
    RETURN_TYPE,
    DATA_TYPE,
    BOOLEAN,
    INTEGER,
    NEGATIVE_INTEGER,
};

/**
 * @brief O(1) lookup table for string to enum conversion of all the Non-Terminals.
 */
const vector<string> non_terminal_names = {
    // "NONE_NON_TERMINAL",
    "PROGRAM",
    "PROGRAM",
    "FUNCTIONS",
    "FUNCTION",
    "FUNCTION_PARAMETERS",
    "PARAMETERS",
    "MORE_PARAMETERS",
    "PARAMETER",
    "COMPOUND_STATEMENTS",
    "STATEMENT",
    "LOOP_STATEMENT",
    "CONDITIONAL_STATEMENTS",
    "ELSE_STATEMENT",
    "BREAK_STATEMENTS",
    "RETURN_ENTITY",
    "FUNCTION_CALL",
    "ARGUEMENTS",
    "ADDITIONAL_ARGUMENTS",
    "INITIALIZATION_STATEMENTS",
    "ASSIGN_STATEMENT",
    "TERMIMAL_EXPRESSIONS",
    "ADDITIONAL_TERMINALS",
    "EXPRESSION",
    "LOGICAL_EXPRESSION",
    "RELATIONAL_EXPRESSION",
    "RELATION_OPERATION",
    "MORE_REL_OP",
    "NUMERIC_EXPRESSION",
    "NUMERIC_OPERATION",
    "MULTIPLICATION_EXPRESSION",
    "MULTIPLICATION_OPERATION",
    "REL_OP",
    "REL_EQ_OP",
    "REL_INEQ_OP",
    "TERMIMAL_EXPRESSION",
    "TERMINAL_ID",
    "RETURN_TYPE",
    "DATA_TYPE",
    "BOOLEAN",
    "INTEGER",
    "NEGATIVE_INTEGER",
};

/**
 * @brief Defines they type of the parser node value as Terminal node or Non-Terminal node.
 */
enum ParserTokenType {
    TERMINAL,
    NON_TERMINAL
};

/**
 * @brief Object to hold either a Terminal value or Non-Terminal value.
 */
typedef struct ParserTypeValue {
    TERMINALS terminal;
    NON_TERMINALS nonTerminal;
    ParserTypeValue() {}
    ParserTypeValue(TERMINALS terminal, NON_TERMINALS nonTerminal) {
        this->terminal = terminal;
        this->nonTerminal = nonTerminal;
    }

    const bool operator==(const ParserTypeValue& other) const {
        return tie(this->terminal, this->nonTerminal) == tie(other.terminal, other.nonTerminal);
    }

    const bool operator<(const ParserTypeValue& other) const {
        return tie(this->terminal, this->nonTerminal) < tie(other.terminal, other.nonTerminal);
    }

    const bool operator>(const ParserTypeValue& other) const {
        return tie(this->terminal, this->nonTerminal) > tie(other.terminal, other.nonTerminal);
    }
} ParserTypeValue;

typedef pair<ParserTokenType, ParserTypeValue> ppp;

const ppp EPSILON = make_pair(ParserTokenType::TERMINAL, ParserTypeValue(TERMINALS::EMPTY, NON_TERMINALS::NONE_NON_TERMINAL));
const ppp _END_MARKER = make_pair(ParserTokenType::TERMINAL, ParserTypeValue(TERMINALS::END_MARKER, NON_TERMINALS::NONE_NON_TERMINAL));

/**
 * @brief A single production rule in the grammar that defines what a Non-Terminal exapnds to.
 * 
 */
typedef struct ProductionRule {
    //!
    // NON_TERMINALS lhs;
    ppp lhs;

    //!
    vector<pair<ParserTokenType, ParserTypeValue>> rhs;

    //!
    bool synchProduction;
    bool skipProduction;

    /**
	 * @brief Construct a new Production Rule object
	 * 
	 * @param lhs 
	 */
    ProductionRule(ppp lhs) {
        this->lhs = lhs;
        this->rhs = vector<pair<ParserTokenType, ParserTypeValue>>();
        this->synchProduction = false;
    }

    ProductionRule(bool synch) {
        this->synchProduction = synch;
        this->skipProduction = false;
    }

    ProductionRule() {
        // create Skip Production
        this->synchProduction = false;
        this->skipProduction = true;
    }

    void print() {
        if (this->lhs.first == ParserTokenType::TERMINAL) {
            cout << terminal_names[this->lhs.second.terminal];
        } else {
            cout << non_terminal_names[this->lhs.second.nonTerminal];
        }

        cout << " -> ";

        if (this->rhs[0].first == ParserTokenType::TERMINAL) {
            cout << terminal_names[this->rhs[0].second.terminal];
        } else {
            cout << non_terminal_names[this->rhs[0].second.nonTerminal];
        }
    }

    const bool operator==(const ProductionRule& other) const {
        return tie(this->lhs, this->rhs, this->synchProduction) == tie(other.lhs, other.rhs, other.synchProduction);
    }

    const bool operator<(const ProductionRule& other) const {
        return tie(this->lhs, this->rhs, this->synchProduction) < tie(other.lhs, other.rhs, other.synchProduction);
    }

    const bool operator>(const ProductionRule& other) const {
        return tie(this->lhs, this->rhs, this->synchProduction) > tie(other.lhs, other.rhs, other.synchProduction);
    }
} ProductionRule;

const ProductionRule SYNCH_PRODUCTION = ProductionRule(true);
const ProductionRule SKIP_PRODUCTION = ProductionRule();

/**
 * @brief 
 */
typedef struct ParserNode {
    ppp lexerValueTokenTypeIdentifier;
    // ParserTokenType type;
    // ParserTypeValue value;
    Token* lexerToken;

    ParserNode(ppp lexerValueTokenTypeIdentifier, Token* lexerToken) {
        this->lexerValueTokenTypeIdentifier = lexerValueTokenTypeIdentifier;
        this->lexerToken = lexerToken;
    }
} ParserNode;

enum SyntaxErrorType {
    SKIP,
    SYNCH,
};

string terminalLookup(TERMINALS terminals);

typedef struct SyntaxError {
    SyntaxErrorType errorType;
    ParserNode parserNode;
    ppp expected;
    ppp received;

    SyntaxError(ParserNode parserNode, ppp expected, ppp received) : parserNode{parserNode} {
        this->errorType = SKIP;
        this->expected = expected;
        this->received = received;
    }

    SyntaxError(ParserNode parserNode) : parserNode{parserNode} {
        this->errorType = SYNCH;
    }

    void print() {
        if (this->errorType == SyntaxErrorType::SKIP) {
            cout << parserNode.lexerToken->filename << ":" << parserNode.lexerToken->line_number << ":" << parserNode.lexerToken->columnNumber << ": ";
            cout << "\033[1;31mSyntax Error: \033[0m";
            if (received.second.terminal == TERMINALS::FUNCTION_IDENTIFIER || received.second.terminal == TERMINALS::IDENTIFIER || received.second.terminal == TERMINALS::POSITIVE_INTEGER) {
                cout << "Expected \'" << terminalLookup(expected.second.terminal) << "\' but received \'" << parserNode.lexerToken->lexeme << "\'" << endl;
            } else {
                cout << "Expected \'" << terminalLookup(expected.second.terminal) << "\' but received \'" << terminalLookup(received.second.terminal) << "\'" << endl;
            }

            cout << "  " << parserNode.lexerToken->line_number << " | " << parserNode.lexerToken->buffer << "\n";
            // cout << "  " << parserNode.lexerToken->line_number << " | ";
            // for (unsigned int i = 0; i < parserNode.lexerToken->columnNumber; ++i) {
            //     cout << " ";
            // }
            // cout << "^";
            // for (unsigned int i = 0; i < (parserNode.lexerToken->tokenLength - 1); ++i) {
            //     cout << "~";
            // }
            cout << endl;
        } else if (this->errorType == SyntaxErrorType::SYNCH) {
            cout << parserNode.lexerToken->filename << ":" << parserNode.lexerToken->line_number << ": ";  //":" << parserNode.lexerToken->columnNumber << ": ";
            cout << "\033[1;31mSyntax Error: \033[0m"
                 << "\n";
            // cout << "Expected " << terminal_names[expected.second.terminal] << ", but received " << terminal_names[received.second.terminal] << endl;
            cout << "  " << parserNode.lexerToken->line_number << " | " << parserNode.lexerToken->buffer << "\n";
            // cout << "  " << parserNode.lexerToken->line_number << " | ";
            // for (unsigned int i = 0; i < parserNode.lexerToken->columnNumber; ++i) {
            //     cout << " ";
            // }
            // cout << "^";
            // for (unsigned int i = 0; i < (parserNode.lexerToken->tokenLength - 1); ++i) {
            //     cout << "~";
            // }
            cout << endl;
        }
    }
} SyntaxError;

// typedef struct SyntaxError_Synch {
//     ParserNode parserNode;

//     SyntaxError_Synch(ParserNode parserNode) : parserNode{parserNode} {}

//     void print() {
//         cout << parserNode.lexerToken->filename << ":" << parserNode.lexerToken->line_number;  //":" << parserNode.lexerToken->columnNumber << ": ";
//         cout << "\033[1;31mSyntax Error: \033[0m" << endl;
//         // cout << "Expected " << terminal_names[expected.second.terminal] << ", but received " << terminal_names[received.second.terminal] << endl;
//         cout << "  " << parserNode.lexerToken->line_number << " | " << parserNode.lexerToken->buffer << endl;
//         // cout << "  " << parserNode.lexerToken->line_number << " | ";
//         // for (unsigned int i = 0; i < parserNode.lexerToken->columnNumber; ++i) {
//         //     cout << " ";
//         // }
//         // cout << "^";
//         // for (unsigned int i = 0; i < (parserNode.lexerToken->tokenLength - 1); ++i) {
//         //     cout << "~";
//         // }
//         cout << endl;
//     }
// } SyntaxError_Synch;

/**
 * @brief 
 */
typedef struct Parser {
    Lexer* lexer;
    vector<Token*> tokenList;
    vector<ParserNode*> parserTokenList;
    vector<LexerError*> lexerErrors;

    fstream rules_file;
    vector<ProductionRule> grammarRules;
    // First Set
    // map<pair<ParserTokenType, ParserTypeValue>, vector<pair<ParserTokenType, ParserTypeValue>>> firstSet;
    map<ppp, vector<ppp>> firstSet;
    map<ppp, vector<ppp>> followSet;
    map<ppp, map<ppp, ProductionRule>> parseTable;

    /*
	1) Print all the Grammar rule : i) X -> ABCDEFGHI
	   +-------------------------------------------+
	   | a  b    b   b b  b  b b                           |
	A    X -> ABCDEFGHI	     2   
	B
	C
	D
	E
	F
	G
	H
	I
	
	*/
    // Follow Set
    // Parse Table
    vector<ppp> parseStack;
    vector<SyntaxError> syntaxErrors;
    vector<vector<ppp>> stackLMD;

    Parser(string filename) {
        // initTerminalLookup();
        this->lexer = initLexer(filename);
        this->tokenList = vector<Token*>();
        this->parserTokenList = vector<ParserNode*>();
        this->lexerErrors = vector<LexerError*>();
        this->grammarRules = vector<ProductionRule>();
        this->firstSet = map<ppp, vector<ppp>>();
        this->followSet = map<ppp, vector<ppp>>();
        this->parseStack = vector<ppp>();
        this->parseStack.push_back(_END_MARKER);
        // this->parseStack.push_back(make_pair(ParserTokenType::NON_TERMINAL, ParserTypeValue(TERMINALS::NONE_TERMINAL, NON_TERMINALS::PROGRAM)));
    }
} Parser;

void printStackProductions(vector<ppp>);

void printTokenMessage(Token* token);

/**
 * @brief Takes as input a string type object and returns the parser object.
 * 
 * @param filename 
 * @return Parser* 
 */
Parser* initializeParser(string filename);

/**
 * @brief Takes as input a parser object and accumalates all the tokens, errors.
 * 
 * @param parser 
 */
void runParser(Parser* parser);

/**
 * @brief Read from the rules.bnf file and create a vector of Production Rules
 * 
 * @param parser 
 */
void productionsRulesInit(Parser* parser);

/**
 * @brief Converts the lexer token type to a parser terminal type.
 * 
 * @param lexerToken 
 * @return ParserNode* 
 */
ParserNode* determineParserNode(Token* lexerToken);

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool isElement(string, vector<string>);

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool isPPPElement(ppp, vector<ppp>&);

bool isProductionRuleElement(ProductionRule, vector<ProductionRule>&);

/**
 * @brief 
 * 
 * @return TERMINALS 
 */
TERMINALS convertStringToT(string);

/**
 * @brief 
 * 
 * @return NON_TERMINALS 
 */
NON_TERMINALS convertStringToNT(string);

/**
 * @brief 
 * 
 * @param line 
 * @param delim 
 * @return vector<string> 
 */
vector<string> split(const string& line, const string& delim);

bool addUnique(ppp, vector<ppp>&);
bool collectFirsts4(map<ppp, vector<ppp>>&, vector<ppp>&, vector<ppp>&);

void generateFirstSets(Parser*);

vector<ppp> collectFirsts3(map<ppp, vector<ppp>>, vector<ppp>);
void generateFollowSets(Parser*);

void generateParseTable(Parser*);

template <class T>
bool isElement(T element, vector<T> arr);

// #ifdef __cplusplus
// }
// #endif
#endif