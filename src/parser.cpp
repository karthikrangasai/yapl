#include <algorithm>
#include <cassert>
#include <sys/stat.h>
#include "parser.hpp"

void printTables(Parser* parser, string filename, int flag) {
    fstream f(filename, fstream::out);

    if (flag == 0) {
        unsigned int columnWidth = 30;
        string temp;
        string spaces(columnWidth - temp.length(), ' ');
        for (auto const& it : parser->firstSet) {
            ppp key = it.first;
            if (key.first == ParserTokenType::TERMINAL) {
                temp = terminal_names[key.second.terminal];
                spaces = string(columnWidth - temp.length(), ' ');
                f << temp << spaces;
            } else {
                temp = non_terminal_names[key.second.nonTerminal];
                spaces = string(columnWidth - temp.length(), ' ');
                f << temp << spaces;
            }

            f << "  -->  ";

            vector<ppp> s = it.second;
            f << "[" << s.size() << "]"
              << "  {";
            for (auto const& iit : s) {
                if (iit.first == ParserTokenType::TERMINAL) {
                    f << terminal_names[iit.second.terminal];
                } else {
                    f << non_terminal_names[iit.second.nonTerminal];
                }
                f << ", ";
            }

            f << "}" << endl
              << endl;
        }
    } else if (flag == 1) {
        unsigned int columnWidth = 30;
        string temp;
        string spaces(columnWidth - temp.length(), ' ');
        for (auto const& it : parser->followSet) {
            ppp key = it.first;
            if (key.first == ParserTokenType::TERMINAL) {
                temp = terminal_names[key.second.terminal];
                spaces = string(columnWidth - temp.length(), ' ');
                f << temp << spaces;
            } else {
                temp = non_terminal_names[key.second.nonTerminal];
                spaces = string(columnWidth - temp.length(), ' ');
                f << temp << spaces;
            }

            f << "  -->  ";

            vector<ppp> s = it.second;
            f << "[" << s.size() << "]"
              << "  {";
            for (auto const& iit : s) {
                if (iit.first == ParserTokenType::TERMINAL) {
                    f << terminal_names[iit.second.terminal];
                } else {
                    f << non_terminal_names[iit.second.nonTerminal];
                }
                f << ", ";
            }

            f << "}" << endl
              << endl;
        }
    } else if (flag == 2) {
        unsigned int columnWidth = 30;
        string rowSeparator(terminal_names.size() * (columnWidth + 1) + 1, '-');
        char separator = '|';
        string temp;
        string spaces(columnWidth - temp.length(), ' ');

        // Print Column Headers
        f << temp << separator << spaces;
        for (unsigned int i = 1; i < terminal_names.size(); ++i) {
            temp = terminal_names[i];
            string left_spaces = string((columnWidth - temp.length()) / 2, ' ');
            string right_spaces = string((columnWidth - temp.length() + 1) / 2, ' ');
            f << separator << left_spaces << temp << right_spaces;
        }
        f << separator << endl
          << rowSeparator << endl;

        // Printing rows
        for (unsigned int i = 0; i < non_terminal_names.size(); ++i) {
            NON_TERMINALS nt = convertStringToNT(non_terminal_names[i]);
            ppp nonterminal = make_pair(ParserTokenType::NON_TERMINAL, ParserTypeValue(TERMINALS::NONE_TERMINAL, nt));

            // ppp nonterminal = parser->grammarRules[i].lhs;
            temp = non_terminal_names[nonterminal.second.nonTerminal];
            string left_spaces = string((columnWidth - temp.length()) / 2, ' ');
            string right_spaces = string((columnWidth - temp.length() + 1) / 2, ' ');
            f << separator << left_spaces << temp << right_spaces;

            for (unsigned int j = 1; j < terminal_names.size(); ++j) {
                TERMINALS t = convertStringToT(terminal_names[j]);
                ppp terminal = make_pair(ParserTokenType::TERMINAL, ParserTypeValue(t, NON_TERMINALS::NONE_NON_TERMINAL));

                if (parser->parseTable[nonterminal][terminal] == SKIP_PRODUCTION) {
                    temp = "SKIP";
                } else if (parser->parseTable[nonterminal][terminal] == SYNCH_PRODUCTION) {
                    temp = "SYNCH";
                } else {
                    unsigned int index = distance(parser->grammarRules.begin(), find(parser->grammarRules.begin(), parser->grammarRules.end(), parser->parseTable[nonterminal][terminal]));
                    temp = to_string(index);
                }
                string left_spaces = string((columnWidth - temp.length()) / 2, ' ');
                string right_spaces = string((columnWidth - temp.length() + 1) / 2, ' ');
                f << separator << left_spaces << temp << right_spaces;
            }
            f << separator << endl
              << rowSeparator << endl;
        }
    }
    f.close();
}

Parser* initializeParser(string filename) {
    Parser* parser = new Parser(filename);
    productionsRulesInit(parser);
    generateFirstSets(parser);
    printTables(parser, "./src/parse_table/firstSet.txt", 0);
    generateFollowSets(parser);
    printTables(parser, "./src/parse_table/followSet.txt", 1);
    generateParseTable(parser);
    printTables(parser, "./src/parse_table/parserTable.txt", 2);
    return parser;
}

void printStackProductions(vector<ppp> stackInputs) {
    for (auto it : stackInputs) {
        if (it.first == ParserTokenType::TERMINAL)
            cout << terminal_names[it.second.terminal] << " ";
        else
            cout << non_terminal_names[it.second.nonTerminal] << " ";
    }
    cout << endl
         << endl;
}

void storeStackProductions(Parser* parser, vector<ppp> stackInputs) {
    parser->stackLMD.push_back(stackInputs);
}

void runParser(Parser* parser) {
    // Token collection
    do {
        getNextToken(parser->lexer);
        if (parser->lexer->lexerState == FOUND_TOKEN) {
            parser->tokenList.push_back(parser->lexer->currentToken);
            parser->parserTokenList.push_back(determineParserNode(parser->lexer->currentToken));
            parser->lexer->resetGotToken();
        } else if (parser->lexer->lexerState == ERROR_OCCURED) {
            parser->lexerErrors.push_back(parser->lexer->currentLexerError);
            parser->lexer->reseterrorOccured();
        }
    } while (!(parser->lexer->reachedEOF && ((parser->lexer->currPtr >= parser->lexer->bufferLen))));

    // You have all the details to run LL(1) Parser
    unsigned int tokenIndex = 0;
    parser->parseStack.push_back(parser->grammarRules[0].lhs);
    ppp X = parser->parseStack.back();
    ppp a = parser->parserTokenList[tokenIndex]->lexerValueTokenTypeIdentifier;
    storeStackProductions(parser, parser->parseStack);
    while (!(X == _END_MARKER)) {
        if (X == a) {
            // pop the stack and let a be the next symbol of w ;
            parser->parseStack.pop_back();
            ++tokenIndex;
            if (tokenIndex < parser->parserTokenList.size()) {
                a = parser->parserTokenList[tokenIndex]->lexerValueTokenTypeIdentifier;
            } else {
                a = _END_MARKER;
            }
        } else if (X.first == ParserTokenType::TERMINAL) {
            if (X.second.terminal == TERMINALS::EMPTY) {
                parser->parseStack.pop_back();
            } else {
                // Expected X, Got a.
                if (tokenIndex < parser->parserTokenList.size()) {
                    parser->syntaxErrors.push_back(SyntaxError(*(parser->parserTokenList[tokenIndex]), X, a));
                } else {
                    parser->syntaxErrors.push_back(SyntaxError(*(parser->parserTokenList.back()), X, a));
                }
                parser->parseStack.pop_back();
            }
        } else if (parser->parseTable[X][a] == SKIP_PRODUCTION) {
            // skip a
            if (a == _END_MARKER) {
                parser->parseStack.pop_back();
            } else {
                ++tokenIndex;
                if (tokenIndex < parser->parserTokenList.size()) {
                    parser->syntaxErrors.push_back(SyntaxError(*(parser->parserTokenList[tokenIndex]), a));
                    a = parser->parserTokenList[tokenIndex]->lexerValueTokenTypeIdentifier;
                } else {
                    parser->syntaxErrors.push_back(SyntaxError(*(parser->parserTokenList.back()), a));
                    parser->parseStack.pop_back();
                }
            }
        } else if (!(parser->parseTable[X][a] == SKIP_PRODUCTION)) {
            // output the production
            if (SYNCH_PRODUCTION == parser->parseTable[X][a]) {
                // SYNCH Error = pop() : skip the production
                parser->parseStack.pop_back();
                parser->syntaxErrors.push_back(SyntaxError(*(parser->parserTokenList[tokenIndex - 1])));
            } else {
                parser->parseStack.pop_back();
                vector<ppp> productions = parser->parseTable[X][a].rhs;
                reverse(productions.begin(), productions.end());
                parser->parseStack.insert(parser->parseStack.end(), productions.begin(), productions.end());
            }
        }
        X = parser->parseStack.back();
        storeStackProductions(parser, parser->parseStack);
    }
}

void productionsRulesInit(Parser* parser) {
    // Assuming filename is always `final_rules.bnf`
    try {
        parser->rules_file.open("./src/parse_table/final_rules.bnf", fstream::in);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    string ruleBuffer;
    getline(parser->rules_file, ruleBuffer);
    while (!(parser->rules_file.eof())) {
        if (ruleBuffer != "") {
            size_t pos = ruleBuffer.find(" -> ");
            if (pos != string::npos) {
                string lhs = ruleBuffer.substr(0, pos);
                string rhs = ruleBuffer.substr(pos + 4);

                ParserTypeValue lhsValue(TERMINALS::NONE_TERMINAL, convertStringToNT(lhs));
                ProductionRule pRule = ProductionRule(make_pair(ParserTokenType::NON_TERMINAL, lhsValue));
                if (rhs == "\'\'") {
                    pRule.rhs.push_back(EPSILON);
                } else {
                    vector<string> vs = split(rhs, " ");

                    // Create a production rule object for the current production rule.
                    for (string& s : vs) {
                        ParserTypeValue value;
                        if (isElement(s, terminal_names)) {
                            pRule.rhs.push_back(make_pair(ParserTokenType::TERMINAL, ParserTypeValue(convertStringToT(s), NON_TERMINALS::NONE_NON_TERMINAL)));
                        } else if (isElement(s, non_terminal_names)) {
                            pRule.rhs.push_back(make_pair(ParserTokenType::NON_TERMINAL, ParserTypeValue(TERMINALS::NONE_TERMINAL, convertStringToNT(s))));
                        }
                    }
                }
                // Store the current rule with all rules.
                parser->grammarRules.push_back(pRule);
            } else {
                // Error case: Invalid Grammar
                cout << "[ERROR]: Invalid Grammar" << endl;
            }
        }
        getline(parser->rules_file, ruleBuffer);
    }
}

ParserNode* determineParserNode(Token* lexerToken) {
    ParserTokenType nodeType = ParserTokenType::TERMINAL;
    ParserTypeValue terminalType(TERMINALS::NONE_TERMINAL, NON_TERMINALS::NONE_NON_TERMINAL);
    switch (lexerToken->token) {
        // case TOKEN_TYPE::BITWISE_NOT: {
        //     terminalType.terminal = TERMINALS::BITWISE_NOT_OPERATOR;
        //     break;
        // }
        // case TOKEN_TYPE::BITWISE_AND: {
        //     terminalType.terminal = TERMINALS::BITWISE_AND_OPERATOR;
        //     break;
        // }
        // case TOKEN_TYPE::BITWISE_OR: {
        //     terminalType.terminal = TERMINALS::BITWISE_OR_OPERATOR;
        //     break;
        // }
        case TOKEN_TYPE::MODULO: {
            terminalType.terminal = TERMINALS::MODULO_OPERATOR;
            break;
        }
        case TOKEN_TYPE::DIVIDE: {
            terminalType.terminal = TERMINALS::DIVISION_OPERATOR;
            break;
        }
        case TOKEN_TYPE::MULTIPLY: {
            terminalType.terminal = TERMINALS::MULTIPLICATION_OPERATOR;
            break;
        }
        case TOKEN_TYPE::ADD: {
            terminalType.terminal = TERMINALS::ADDITION_OPERATOR;
            break;
        }
        case TOKEN_TYPE::SUBTRACT: {
            terminalType.terminal = TERMINALS::SUBTRACTION_OPERATOR;
            break;
        }
        case TOKEN_TYPE::LESS_OR_EQUAL: {
            terminalType.terminal = TERMINALS::LESS_THAN_EQUAL_OPERATOR;
            break;
        }
        case TOKEN_TYPE::GREATER_OR_EQUAL: {
            terminalType.terminal = TERMINALS::GREATER_THAN_EQUAL_OPERATOR;
            break;
        }
        case TOKEN_TYPE::LESS: {
            terminalType.terminal = TERMINALS::LESS_THAN_OPERATOR;
            break;
        }
        case TOKEN_TYPE::GREATER: {
            terminalType.terminal = TERMINALS::GREATER_THAN_OPERATOR;
            break;
        }
        case TOKEN_TYPE::EQUAL: {
            terminalType.terminal = TERMINALS::EQAULITY_OPERATOR;
            break;
        }
        case TOKEN_TYPE::NOTEQUAL: {
            terminalType.terminal = TERMINALS::INEQUALITY_OPERATOR;
            break;
        }
        // case TOKEN_TYPE::LOGICAL_NOT: {
        //     terminalType.terminal = TERMINALS::NOT_OPERATOR;
        //     break;
        // }
        // case TOKEN_TYPE::LOGICAL_AND: {
        //     terminalType.terminal = TERMINALS::AND_OPERATOR;
        //     break;
        // }
        // case TOKEN_TYPE::LOGICAL_OR: {
        //     terminalType.terminal = TERMINALS::OR_OPERATOR;
        //     break;
        // }
        case TOKEN_TYPE::BINDING: {
            terminalType.terminal = TERMINALS::BINDING_OPERATOR;
            break;
        }
        case TOKEN_TYPE::ASSIGNMENT: {
            terminalType.terminal = TERMINALS::ASSIGNMENT_OPERATOR;
            break;
        }
        case TOKEN_TYPE::LEFT_PAREN: {
            terminalType.terminal = TERMINALS::LEFT_PARENTHESES;
            break;
        }
        case TOKEN_TYPE::RIGHT_PAREN: {
            terminalType.terminal = TERMINALS::RIGHT_PARENTHESES;
            break;
        }
        // case TOKEN_TYPE::LEFT_SQUARE_BR: {
        //     terminalType.terminal = TERMINALS::LEFT_SQUARE_BRACKET;
        //     break;
        // }
        // case TOKEN_TYPE::RIGHT_SQUARE_BR: {
        //     terminalType.terminal = TERMINALS::RIGHT_SQUARE_BRACKET;
        //     break;
        // }
        case TOKEN_TYPE::LEFT_FLOWER_BR: {
            terminalType.terminal = TERMINALS::LEFT_FLOWER_BRACKET;
            break;
        }
        case TOKEN_TYPE::RIGHT_FLOWER_BR: {
            terminalType.terminal = TERMINALS::RIGHT_FLOWER_BRACKET;
            break;
        }
        case TOKEN_TYPE::COMMA: {
            terminalType.terminal = TERMINALS::COMMA_SEPARATOR;
            break;
        }
        case TOKEN_TYPE::SEMI_COLON: {
            terminalType.terminal = TERMINALS::SEMI_COLON_SEPARATOR;
            break;
        }
        case TOKEN_TYPE::VAR_IDENTIFIER: {
            terminalType.terminal = TERMINALS::IDENTIFIER;
            break;
        }
        case TOKEN_TYPE::FUNC_IDENTIFIER: {
            terminalType.terminal = TERMINALS::FUNCTION_IDENTIFIER;
            break;
        }
        case TOKEN_TYPE::INTEGER_TOKEN: {
            terminalType.terminal = TERMINALS::POSITIVE_INTEGER;
            break;
        }
        // case TOKEN_TYPE::FLOAT_TOKEN: {
        //     terminalType.terminal = TERMINALS::POSITIVE_FLOAT;
        //     break;
        // }
        // case TOKEN_TYPE::PRE_INCEREMENT: {
        //     terminalType.terminal = TERMINALS::PRE_INCEREMENT_OPERATOR;
        //     break;
        // }
        // case TOKEN_TYPE::PRE_DECREMENT: {
        //     terminalType.terminal = TERMINALS::PRE_DECEREMENT_OPERATOR;
        //     break;
        // }
        // case TOKEN_TYPE::STRING_TOKEN: {
        //     terminalType.terminal = TERMINALS::STRING;
        //     break;
        // }
        case TOKEN_TYPE::KEYWORD: {
            if (lexerToken->lexeme == "boolean") {
                terminalType.terminal = TERMINALS::BOOLEAN_KEYWORD;
            } else if (lexerToken->lexeme == "break") {
                terminalType.terminal = TERMINALS::BREAK_KEYWORD;
            } else if (lexerToken->lexeme == "continue") {
                terminalType.terminal = TERMINALS::CONTINUE_KEYWORD;
            } else if (lexerToken->lexeme == "else") {
                terminalType.terminal = TERMINALS::ELSE_KEYWORD;
            } else if (lexerToken->lexeme == "end_definition") {
                terminalType.terminal = TERMINALS::END_DEFINITION_KEYWORD;
            } else if (lexerToken->lexeme == "end") {
                terminalType.terminal = TERMINALS::END_KEYWORD;
            } else if (lexerToken->lexeme == "false") {
                terminalType.terminal = TERMINALS::FALSE_KEYWORD;
            } else if (lexerToken->lexeme == "function") {
                terminalType.terminal = TERMINALS::FUNTION_KEYWORD;
            } else if (lexerToken->lexeme == "if") {
                terminalType.terminal = TERMINALS::IF_KEYWORD;
            } else if (lexerToken->lexeme == "integer") {
                terminalType.terminal = TERMINALS::INTEGER_KEYWORD;
            } else if (lexerToken->lexeme == "return") {
                terminalType.terminal = TERMINALS::RETURN_KEYWORD;
            } else if (lexerToken->lexeme == "true") {
                terminalType.terminal = TERMINALS::TRUE_KEYWORD;
            } else if (lexerToken->lexeme == "void") {
                terminalType.terminal = TERMINALS::VOID_KEYWORD;
            } else if (lexerToken->lexeme == "while") {
                terminalType.terminal = TERMINALS::WHILE_KEYWORD;
            }
            //  else if (lexerToken->lexeme == "float") {
            //     terminalType.terminal = TERMINALS::FLOAT_KEYWORD;
            // } else if (lexerToken->lexeme == "for") {
            //     terminalType.terminal = TERMINALS::FOR_KEYWORD;
            // }
            break;
        }
        default: {
            // Error case
        }
    }
    assert(terminalType.terminal != TERMINALS::NONE_TERMINAL);
    return new ParserNode(make_pair(nodeType, terminalType), lexerToken);
}

string terminalLookup(TERMINALS terminals) {
    if (terminals == TERMINALS::IDENTIFIER) {
        return "ID";
    } else if (terminals == TERMINALS::FUNCTION_IDENTIFIER) {
        return "function_ID";
    } else if (terminals == TERMINALS::FUNTION_KEYWORD) {
        return "function";
    } else if (terminals == TERMINALS::MODULO_OPERATOR) {
        return "%";
    } else if (terminals == TERMINALS::DIVISION_OPERATOR) {
        return "/";
    } else if (terminals == TERMINALS::MULTIPLICATION_OPERATOR) {
        return "*";
    } else if (terminals == TERMINALS::ADDITION_OPERATOR) {
        return "+";
    } else if (terminals == TERMINALS::SUBTRACTION_OPERATOR) {
        return "-";
    } else if (terminals == TERMINALS::LEFT_PARENTHESES) {
        return "(";
    } else if (terminals == TERMINALS::RIGHT_PARENTHESES) {
        return ")";
    }
    //  else if (terminals == TERMINALS::LEFT_SQUARE_BRACKET) {
    //     return "[";
    // } else if (terminals == TERMINALS::RIGHT_SQUARE_BRACKET) {
    //     return "]";
    // }
    else if (terminals == TERMINALS::LEFT_FLOWER_BRACKET) {
        return "{";
    } else if (terminals == TERMINALS::RIGHT_FLOWER_BRACKET) {
        return "}";
    } else if (terminals == TERMINALS::COMMA_SEPARATOR) {
        return ",";
    } else if (terminals == TERMINALS::SEMI_COLON_SEPARATOR) {
        return ";";
    } else if (terminals == TERMINALS::BINDING_OPERATOR) {
        return "::";
    } else if (terminals == TERMINALS::END_DEFINITION_KEYWORD) {
        return "end_definition";
    } else if (terminals == TERMINALS::POSITIVE_INTEGER) {
        return "integer";
    } else if (terminals == TERMINALS::TRUE_KEYWORD) {
        return "true";
    } else if (terminals == TERMINALS::FALSE_KEYWORD) {
        return "false";
    } else if (terminals == TERMINALS::BOOLEAN_KEYWORD) {
        return "boolean";
    } else if (terminals == TERMINALS::INTEGER_KEYWORD) {
        return "integer";
    } else if (terminals == TERMINALS::VOID_KEYWORD) {
        return "void";
    } else if (terminals == TERMINALS::LESS_THAN_EQUAL_OPERATOR) {
        return "<=";
    } else if (terminals == TERMINALS::GREATER_THAN_EQUAL_OPERATOR) {
        return ">=";
    } else if (terminals == TERMINALS::LESS_THAN_OPERATOR) {
        return "<";
    } else if (terminals == TERMINALS::GREATER_THAN_OPERATOR) {
        return ">";
    } else if (terminals == TERMINALS::EQAULITY_OPERATOR) {
        return "=";
    } else if (terminals == TERMINALS::INEQUALITY_OPERATOR) {
        return "=/=";
    }
    // else if (terminals == TERMINALS::NOT_OPERATOR) {
    //     return "!";
    // } else if (terminals == TERMINALS::OR_OPERATOR) {
    //     return "||";
    // }
    //  else if (terminals == TERMINALS::AND_OPERATOR) {
    //     return "&&";
    // }
    else if (terminals == TERMINALS::ASSIGNMENT_OPERATOR) {
        return ":=";
    } else if (terminals == TERMINALS::BREAK_KEYWORD) {
        return "break";
    } else if (terminals == TERMINALS::CONTINUE_KEYWORD) {
        return "continue";
    } else if (terminals == TERMINALS::RETURN_KEYWORD) {
        return "return";
    } else if (terminals == TERMINALS::WHILE_KEYWORD) {
        return "while";
    } else if (terminals == TERMINALS::END_KEYWORD) {
        return "end";
    } else if (terminals == TERMINALS::IF_KEYWORD) {
        return "if";
    } else if (terminals == TERMINALS::ELSE_KEYWORD) {
        return "else";
    } else if (terminals == TERMINALS::END_MARKER) {
        return "unexpected EOF";
    }

    return "";
}

template <class T>
bool isElement(T element, vector<T> arr) {
    return find(arr.begin(), arr.end(), element) != arr.end();
}

TERMINALS convertStringToT(string s) {
    for (unsigned int i = 0; i < terminal_names.size(); ++i) {
        if (terminal_names[i] == s) {
            return TERMINALS(i);
        }
    }
    return TERMINALS::NONE_TERMINAL;
}
NON_TERMINALS convertStringToNT(string s) {
    for (unsigned int i = 0; i < non_terminal_names.size(); ++i) {
        if (non_terminal_names[i] == s) {
            return NON_TERMINALS(i);
        }
    }
    return NON_TERMINALS::NONE_NON_TERMINAL;
}

vector<string> split(const string& line, const string& delim) {
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do {
        pos = line.find(delim, prev);
        if (pos == string::npos) pos = line.length();
        string token = line.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < line.length() && prev < line.length());
    return tokens;
}

void generateFirstSets(Parser* parser) {
    bool notDone;
    unsigned int count = 0;
    for (unsigned int i = 0; i < terminal_names.size(); ++i) {
        ppp p = make_pair(ParserTokenType::TERMINAL, ParserTypeValue(convertStringToT(terminal_names[i]), NON_TERMINALS::NONE_NON_TERMINAL));
        parser->firstSet[p] = {p};
    }
    do {
        notDone = false;
        for (unsigned int i = 0; i < parser->grammarRules.size(); ++i) {
            vector<ppp> nonterminalFirsts = parser->firstSet[parser->grammarRules[i].lhs];
            vector<ppp> development = parser->grammarRules[i].rhs;
            if (development.size() == 1 && development[0] == EPSILON) {
                notDone |= addUnique(EPSILON, nonterminalFirsts);
            } else {
                notDone |= collectFirsts4(parser->firstSet, development, nonterminalFirsts);
            }
            parser->firstSet[parser->grammarRules[i].lhs] = nonterminalFirsts;
        }
        ++count;
    } while (notDone);
}

bool addUnique(ppp element, vector<ppp>& arr) {
    if (!isElement(element, arr)) {
        arr.push_back(element);
        return true;
    }
    return false;
}

bool collectFirsts4(map<ppp, vector<ppp>>& firsts, vector<ppp>& development, vector<ppp>& nonterminalFirsts) {
    bool result = false;
    bool epsilonInSymbolFirsts = true;

    for (auto const& j : development) {
        ppp symbol = j;
        epsilonInSymbolFirsts = false;
        if (symbol.first == ParserTokenType::TERMINAL) {
            result |= addUnique(symbol, nonterminalFirsts);
            break;
        }
        for (auto const& k : firsts[symbol]) {
            ppp first = k;
            epsilonInSymbolFirsts |= (first == EPSILON);
            result |= addUnique(first, nonterminalFirsts);
        }
        if (!epsilonInSymbolFirsts) {
            break;
        }
    }
    if (epsilonInSymbolFirsts) {
        result |= addUnique(EPSILON, nonterminalFirsts);
    }
    return result;
}

void generateFollowSets(Parser* parser) {
    bool notDone;
    do {
        notDone = false;
        for (unsigned int i = 0; i < parser->grammarRules.size(); ++i) {
            ppp nonterminal = parser->grammarRules[i].lhs;
            vector<ppp> development = parser->grammarRules[i].rhs;

            if (i == 0) {
                vector<ppp> nonterminalFollows = parser->followSet[nonterminal];
                notDone |= addUnique(_END_MARKER, nonterminalFollows);
                parser->followSet[nonterminal] = nonterminalFollows;
            }

            for (unsigned int j = 0; j < development.size(); ++j) {
                ppp symbol = development[j];
                if (symbol.first == ParserTokenType::NON_TERMINAL) {
                    vector<ppp> symbolFollows = parser->followSet[symbol];
                    vector<ppp> afterSymbolFirsts = collectFirsts3(parser->firstSet, vector<ppp>(development.begin() + j + 1, development.end()));

                    for (unsigned int k = 0; k < afterSymbolFirsts.size(); ++k) {
                        ppp first = afterSymbolFirsts[k];
                        if (first == EPSILON) {
                            vector<ppp> nonterminalFollows = parser->followSet[nonterminal];
                            for (unsigned int l = 0; l < nonterminalFollows.size(); ++l) {
                                notDone |= addUnique(nonterminalFollows[l], symbolFollows);
                            }
                        } else {
                            notDone |= addUnique(first, symbolFollows);
                        }
                    }
                    parser->followSet[symbol] = symbolFollows;
                }
            }
        }
    } while (notDone);
}

vector<ppp> collectFirsts3(map<ppp, vector<ppp>> firsts, vector<ppp> sequence) {
    vector<ppp> result;
    bool epsilonInSymbolFirsts = true;

    for (unsigned int j = 0; j < sequence.size(); ++j) {
        ppp symbol = sequence[j];
        epsilonInSymbolFirsts = false;
        if (symbol.first == ParserTokenType::TERMINAL) {
            addUnique(symbol, result);
            break;
        }
        for (auto const& k : firsts[symbol]) {
            ppp first = k;
            epsilonInSymbolFirsts |= (first == EPSILON);
            addUnique(first, result);
        }
        epsilonInSymbolFirsts |= (!firsts.count(symbol) || firsts[symbol].empty());
        if (!epsilonInSymbolFirsts) {
            break;
        }
    }
    if (epsilonInSymbolFirsts) {
        addUnique(EPSILON, result);
    }
    return result;
}

void generateParseTable(Parser* parser) {
    // Init of cells to SKIP Production
    for (unsigned int i = 0; i < parser->grammarRules.size(); ++i) {
        ppp nonterminal = parser->grammarRules[i].lhs;

        for (unsigned int j = 0; j < terminal_names.size(); ++j) {
            TERMINALS t = convertStringToT(terminal_names[j]);
            ppp terminal = make_pair(ParserTokenType::TERMINAL, ParserTypeValue(t, NON_TERMINALS::NONE_NON_TERMINAL));

            parser->parseTable[nonterminal][terminal] = SKIP_PRODUCTION;
        }
    }

    for (unsigned int i = 0; i < parser->grammarRules.size(); ++i) {
        ppp nonterminal = parser->grammarRules[i].lhs;
        vector<ppp> development = parser->grammarRules[i].rhs;
        vector<ppp> developmentFirsts = collectFirsts3(parser->firstSet, development);
        for (unsigned int j = 0; j < developmentFirsts.size(); ++j) {
            ppp symbol = developmentFirsts[j];
            if (symbol != EPSILON) {
                parser->parseTable[nonterminal][symbol] = parser->grammarRules[i];
            } else {
                for (unsigned int j = 0; j < parser->followSet[nonterminal].size(); ++j) {
                    ppp symbol2 = parser->followSet[nonterminal][j];
                    parser->parseTable[nonterminal][symbol2] = parser->grammarRules[i];
                }
            }
        }
    }

    // Adding SYNCH_PRODUNCTIONs
    for (unsigned int i = 0; i < parser->grammarRules.size(); ++i) {
        ppp nonterminal = parser->grammarRules[i].lhs;
        vector<ppp> rhs = parser->grammarRules[i].rhs;
        if (!isElement(EPSILON, rhs)) {
            for (unsigned int j = 0; j < parser->followSet[nonterminal].size(); ++j) {
                ppp symbol2 = parser->followSet[nonterminal][j];
                if (parser->parseTable[nonterminal][symbol2] == SKIP_PRODUCTION) {
                    parser->parseTable[nonterminal][symbol2] = SYNCH_PRODUCTION;
                }
            }
        }
    }
}