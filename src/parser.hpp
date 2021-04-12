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

#include "lexer.hpp"
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief List of all the Terminals
 */
// enum TERMINALS {
//     NONE_TERMINAL = -2,
//     EMPTY = -1,

//     BITWISE_NOT_OPERATOR,
//     BITWISE_AND_OPERATOR,
//     BITWISE_OR_OPERATOR,

//     MODULO_OPERATOR,
//     DIVISION_OPERATOR,
//     MULTIPLICATION_OPERATOR,
//     ADDITION_OPERATOR,
//     SUBTRACTION_OPERATOR,

//     LESS_THAN_EQUAL_OPERATOR,
//     GREATER_THAN_EQUAL_OPERATOR,
//     LESS_THAN_OPERATOR,
//     GREATER_THAN_OPERATOR,
//     EQAULITY_OPERATOR,
//     INEQUALITY_OPERATOR,

//     NOT_OPERATOR,
//     OR_OPERATOR,
//     AND_OPERATOR,

//     BINDING_OPERATOR,
//     ASSIGNMENT_OPERATOR,

//     LEFT_PARENTHESES,
//     RIGHT_PARENTHESES,
//     LEFT_SQUARE_BRACKET,
//     RIGHT_SQUARE_BRACKET,
//     LEFT_FLOWER_BRACKET,
//     RIGHT_FLOWER_BRACKET,

//     COMMA_SEPARATOR,
//     SEMI_COLON_SEPARATOR,

//     IDENTIFIER,
//     FUNCTION_IDENTIFIER,

//     POSITIVE_INTEGER,
//     POSITIVE_FLOAT,

//     PRE_INCEREMENT_OPERATOR,
//     PRE_DECEREMENT_OPERATOR,

//     STRING,

//     BOOLEAN_KEYWORD,
//     BREAK_KEYWORD,
//     CONTINUE_KEYWORD,
//     ELSE_KEYWORD,
//     END_DEFINITION_KEYWORD,
//     END_KEYWORD,
//     FALSE_KEYWORD,
//     FLOAT_KEYWORD,
//     FOR_KEYWORD,
//     FUNTION_KEYWORD,
//     IF_KEYWORD,
//     INTEGER_KEYWORD,
//     RETURN_KEYWORD,
//     TRUE_KEYWORD,
//     VOID_KEYWORD,
//     WHILE_KEYWORD,
//     STRING_KEYWORD,  // "string" => STRING_KEYWORD VAR_ID EQ STRING;
// };

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
    LEFT_SQUARE_BRACKET,
    RIGHT_SQUARE_BRACKET,
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
    NOT_OPERATOR,
    OR_OPERATOR,
    AND_OPERATOR,
    ASSIGNMENT_OPERATOR,
    BREAK_KEYWORD,
    CONTINUE_KEYWORD,
    RETURN_KEYWORD,
    WHILE_KEYWORD,
    END_KEYWORD,
    IF_KEYWORD,
    ELSE_KEYWORD,
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
    "LEFT_SQUARE_BRACKET",
    "RIGHT_SQUARE_BRACKET",
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
    "NOT_OPERATOR",
    "OR_OPERATOR",
    "AND_OPERATOR",
    "ASSIGNMENT_OPERATOR",
    "BREAK_KEYWORD",
    "CONTINUE_KEYWORD",
    "RETURN_KEYWORD",
    "WHILE_KEYWORD",
    "END_KEYWORD",
    "IF_KEYWORD",
    "ELSE_KEYWORD",
};

/**
 * @brief List of all the Non-Terminals.
 */
// enum NON_TERMINALS {
//     NONE_NON_TERMINAL = -1,

//     PROGRAM,

//     FUNCTIONS,
//     FUNCTION,
//     MAIN,

//     FUNCTION_PARAMETERS,
//     PARAMETERS,
//     PARAMETER,

//     COMPOUND_STATEMENTS,
//     STATEMENT,

//     BLOCK_STATEMENTS,
//     BLOCK_STATEMENT,
//     CONDITIONAL_STATEMENTS,
//     LOOP_STATEMENTS,

//     SINGLE_LINE_STATEMENT,
//     BREAK_STATEMENTS,
//     FUNCTION_CALL,
//     INITIALIZATION_STATEMENTS,

//     ARGUEMENTS,

//     DECLARATION_STATEMENT,
//     INITIALIZATION_STATEMENT,

//     EXPRESSION,
//     LOGICAL_EXPRESSION,
//     DISJUNCTION_EXPRESSION,
//     CONJUNCTION_EXPRESSION,
//     NEGATION_EXPRESSION,
//     LOGICAL_BRAKET_EXPRESSION,
//     RELATIONAL_EXPRESSION,
//     NUMERIC_EXPRESSION,
//     SUBTRACTION_EXPRESSION,
//     ADDITION_EXPRESSION,
//     MULTIPLICATION_EXPRESSION,
//     DIVISION_EXPRESSION,
//     MODULAR_EXPRESSION,
//     BITWISE_OR_EXPRESSION,
//     BITWISE_AND_EXPRESSION,
//     BITWISE_NEG_EXPRESSION,
//     NUMERIC_BRAKET_EXPRESSION,

//     TERMIMAL_EXPRESSIONS,
//     TERMIMAL_EXPRESSION,

//     ARRAY_IDENTIFIER,
//     ARRAY_INITIAL_VALUE,
//     INITIAL_VALUE,

//     ARITH_STATEMENT,
//     ARITH_U_OP,

//     REL_OP,
//     REL_EQ_OP,
//     REL_INEQ_OP,

//     RETURN_TYPE,
//     DATA_TYPE,
//     BOOLEAN,
//     INTEGER,
//     NEGATIVE_INTEGER,
//     FLOAT,
//     NEGATIVE_FLOAT,
// };

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
    LOGICAL_EXPR_EQUALITY_OPERATION,
    LOGICAL_EXPR_RELATIONAL_OPERATION,
    LOGICAL_AND_OR_OP,
    LOGICAL_BRAKET_EXPRESSION,
    RELATIONAL_EXPRESSION,
    RELATIONAL_OPERATION,
    NUMERIC_EXPRESSION,
    NUMERIC_OPERATION,
    MULTIPLICATION_EXPRESSION,
    MULT_DIV_MOD_OPERATIONS,
    NUMERIC_BRAKET_EXPRESSION,
    REL_OP,
    REL_EQ_OP,
    REL_INEQ_OP,
    TERMIMAL_EXPRESSION,
    TERMINAL_ID,
    ARRAY_IDENTIFIER,
    INSIDE_ARRAY,
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
    "LOGICAL_EXPR_EQUALITY_OPERATION",
    "LOGICAL_EXPR_RELATIONAL_OPERATION",
    "LOGICAL_AND_OR_OP",
    "LOGICAL_BRAKET_EXPRESSION",
    "RELATIONAL_EXPRESSION",
    "RELATIONAL_OPERATION",
    "NUMERIC_EXPRESSION",
    "NUMERIC_OPERATION",
    "MULTIPLICATION_EXPRESSION",
    "MULT_DIV_MOD_OPERATIONS",
    "NUMERIC_BRAKET_EXPRESSION",
    "REL_OP",
    "REL_EQ_OP",
    "REL_INEQ_OP",
    "TERMIMAL_EXPRESSION",
    "TERMINAL_ID",
    "ARRAY_IDENTIFIER",
    "INSIDE_ARRAY",
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

    /**
	 * @brief Construct a new Production Rule object
	 * 
	 * @param lhs 
	 */
    ProductionRule(ppp lhs) {
        this->lhs = lhs;
        this->rhs = vector<pair<ParserTokenType, ParserTypeValue>>();
    }
} ProductionRule;

/**
 * @brief 
 */
typedef struct ParserNode {
    ParserTokenType type;
    ParserTypeValue value;
    Token* lexerToken;

    ParserNode(ParserTokenType type, ParserTypeValue value, Token* lexerToken) {
        this->type = type;
        this->value = value;
        this->lexerToken = lexerToken;
    }
} ParserNode;

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

    // Follow Set
    // Parse Table

    Parser(string filename) {
        this->lexer = initLexer(filename);
        this->tokenList = vector<Token*>();
        this->parserTokenList = vector<ParserNode*>();
        this->lexerErrors = vector<LexerError*>();
        this->grammarRules = vector<ProductionRule>();
        this->firstSet = map<ppp, vector<ppp>>();
    }
} Parser;

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

void generateFollowSets(Parser*);

void generateParseTable(Parser*);

#ifdef __cplusplus
}
#endif
#endif