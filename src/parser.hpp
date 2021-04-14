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

const vector<string> terminal_names = {
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
};

const vector<string> non_terminal_names = {
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
};

enum ParserTokenType {
    TERMINAL,
    NON_TERMINAL
};

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

typedef struct ProductionRule {
    ppp lhs;
    vector<pair<ParserTokenType, ParserTypeValue>> rhs;
    bool synchProduction;
    bool skipProduction;

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

typedef struct ParserNode {
    ppp lexerValueTokenTypeIdentifier;
    Token* lexerToken;

    ParserNode(ppp lexerValueTokenTypeIdentifier, Token* lexerToken) {
        this->lexerValueTokenTypeIdentifier = lexerValueTokenTypeIdentifier;
        this->lexerToken = lexerToken;
    }

    const bool operator==(const ParserNode& other) const {
        return tie(lexerValueTokenTypeIdentifier, lexerToken) == tie(other.lexerValueTokenTypeIdentifier, other.lexerToken);
    }
} ParserNode;

enum SyntaxErrorType {
    SKIP,
    SYNCH,
    MISMATCHED_TERMINALS,
};

string terminalLookup(TERMINALS terminals);

typedef struct SyntaxError {
    SyntaxErrorType errorType;
    ParserNode parserNode;
    ppp expected;
    ppp received;

    SyntaxError(ParserNode parserNode, ppp expected, ppp received) : parserNode{parserNode} {
        this->errorType = MISMATCHED_TERMINALS;
        this->expected = expected;
        this->received = received;
    }

    SyntaxError(ParserNode parserNode) : parserNode{parserNode} {
        this->errorType = SYNCH;
        this->expected = EPSILON;
        this->received = EPSILON;
    }

    SyntaxError(ParserNode parserNode, ppp received) : parserNode{parserNode} {
        this->errorType = SKIP;
        this->received = received;
        this->expected = EPSILON;
    }

    void print() {
        if (this->errorType == SyntaxErrorType::MISMATCHED_TERMINALS) {
            cout << parserNode.lexerToken->filename << ":" << parserNode.lexerToken->line_number << ":" << parserNode.lexerToken->columnNumber << ": ";
            cout << "\033[1;31mSyntax Error: \033[0m";
            if (received.second.terminal == TERMINALS::FUNCTION_IDENTIFIER || received.second.terminal == TERMINALS::IDENTIFIER || received.second.terminal == TERMINALS::POSITIVE_INTEGER) {
                cout << "Expected \'" << terminalLookup(expected.second.terminal) << "\' but received \'" << parserNode.lexerToken->lexeme << "\'" << endl;
            } else {
                cout << "Expected \'" << terminalLookup(expected.second.terminal) << "\' but received \'" << terminalLookup(received.second.terminal) << "\'" << endl;
            }
            cout << "  " << parserNode.lexerToken->line_number << " | " << parserNode.lexerToken->buffer << "\n";
            cout << endl;
        } else if (this->errorType == SyntaxErrorType::SYNCH) {
            cout << parserNode.lexerToken->filename << ":" << parserNode.lexerToken->line_number << ": ";
            cout << "\033[1;31mSyntax Error: \033[0m"
                 << "\n";
            cout << "  " << parserNode.lexerToken->line_number << " | " << parserNode.lexerToken->buffer << "\n";
            cout << endl;
        } else if (this->errorType == SyntaxErrorType::SKIP) {
            cout << parserNode.lexerToken->filename << ":" << parserNode.lexerToken->line_number << ": ";
            cout << "\033[1;31mSyntax Error: \033[0m"
                 << "\n";
            cout << "  " << parserNode.lexerToken->line_number << " | " << parserNode.lexerToken->buffer << "\n";
        }
    }
} SyntaxError;

typedef struct Parser {
    Lexer* lexer;
    vector<Token*> tokenList;
    vector<ParserNode*> parserTokenList;
    vector<LexerError*> lexerErrors;

    fstream rules_file;
    vector<ProductionRule> grammarRules;

    //! First Set
    map<ppp, vector<ppp>> firstSet;

    //! Follow Set
    map<ppp, vector<ppp>> followSet;

    //! Parse Table for the LL(1) Grammar
    map<ppp, map<ppp, ProductionRule>> parseTable;

    //! Parsing Stack
    vector<ppp> parseStack;

    //! List of all the Syntax Errors
    vector<SyntaxError> syntaxErrors;

    //! The Left Most Derivation of the current input string.
    vector<vector<ppp>> stackLMD;

    Parser(string filename) {
        this->lexer = initLexer(filename);
        this->tokenList = vector<Token*>();
        this->parserTokenList = vector<ParserNode*>();
        this->lexerErrors = vector<LexerError*>();
        this->grammarRules = vector<ProductionRule>();
        this->firstSet = map<ppp, vector<ppp>>();
        this->followSet = map<ppp, vector<ppp>>();
        this->parseStack = vector<ppp>();
        this->parseStack.push_back(_END_MARKER);
    }
} Parser;

void printStackProductions(vector<ppp>);

Parser* initializeParser(string filename);

TERMINALS convertStringToT(string);
NON_TERMINALS convertStringToNT(string);

void productionsRulesInit(Parser* parser);

ParserNode* determineParserNode(Token* lexerToken);
vector<string> split(const string& line, const string& delim);
template <class T>
bool isElement(T element, vector<T> arr);
bool addUnique(ppp, vector<ppp>&);
bool collectFirsts4(map<ppp, vector<ppp>>&, vector<ppp>&, vector<ppp>&);
vector<ppp> collectFirsts3(map<ppp, vector<ppp>>, vector<ppp>);
void generateFirstSets(Parser*);
void generateFollowSets(Parser*);
void generateParseTable(Parser*);

void runParser(Parser* parser);
#endif