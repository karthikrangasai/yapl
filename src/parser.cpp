/**
 * @file parser.cpp
 * @brief 
 * 
 */
#include <algorithm>
#include <cassert>
#include "parser.hpp"

void printFirstTable(Parser* parser, string filename, int flag) {
    freopen(filename.c_str(), "w", stdout);

    if (flag == 0) {
        for (auto const& it : parser->firstSet) {
            ppp key = it.first;
            if (key.first == ParserTokenType::TERMINAL) {
                cout << terminal_names[key.second.terminal];
            } else {
                cout << non_terminal_names[key.second.nonTerminal];
            }

            cout << "  -->  ";

            vector<ppp> s = it.second;
            cout << s.size() << "  {";
            for (auto const& iit : s) {
                if (iit.first == ParserTokenType::TERMINAL) {
                    cout << terminal_names[iit.second.terminal];
                } else {
                    cout << non_terminal_names[iit.second.nonTerminal];
                }
                cout << ", ";
            }

            cout << "}" << endl
                 << endl;
        }
    } else if (flag == 1) {
        for (auto const& it : parser->followSet) {
            ppp key = it.first;
            if (key.first == ParserTokenType::TERMINAL) {
                cout << terminal_names[key.second.terminal];
            } else {
                cout << non_terminal_names[key.second.nonTerminal];
            }

            cout << "  -->  ";

            vector<ppp> s = it.second;
            cout << s.size() << "  {";
            for (auto const& iit : s) {
                if (iit.first == ParserTokenType::TERMINAL) {
                    cout << terminal_names[iit.second.terminal];
                } else {
                    cout << non_terminal_names[iit.second.nonTerminal];
                }
                cout << ", ";
            }

            cout << "}" << endl
                 << endl;
        }
    } else if (flag == 2) {
        // Print parse table
        // NT
        // 		T : PR+

        // for (auto const& it : parser->firstSet) {
        // }
    }

    fclose(stdout);
}

/**
 * @brief 
 * 
 * @param filename 
 * @return Parser* 
 */
Parser* initializeParser(string filename) {
    Parser* parser = new Parser(filename);
    productionsRulesInit(parser);
    generateFirstSets(parser);
    // printFirstTable(parser, "firstSet2.txt", 0);
    generateFollowSets(parser);
    // printFirstTable(parser, "followSet2.txt", 1);
    generateParseTable(parser);
    // printFirstTable(parser, "parserTable2.txt", 2);
    return parser;
}

void printStackProductions(vector<ppp> stackInputs) {
    for (auto it : stackInputs) {
        if (it.first == ParserTokenType::TERMINAL)
            cout << terminal_names[it.second.terminal] << " ";
        else
            cout << non_terminal_names[it.second.nonTerminal] << " ";
    }
    cout << endl;
}

void runParser(Parser* parser) {
    // Token collection
    do {
        getNextToken(parser->lexer);
        if (parser->lexer->lexerState == FOUND_TOKEN) {
            // Token* token = new Token(*(parser->lexer->currentToken));
            // parser->tokenList.push_back(token);
            parser->tokenList.push_back(parser->lexer->currentToken);
            parser->parserTokenList.push_back(determineParserNode(parser->lexer->currentToken));
            parser->lexer->resetGotToken();
        } else if (parser->lexer->lexerState == ERROR_OCCURED) {
            // LexerError* lexerError = new LexerError(*(parser->lexer->currentLexerError));
            // parser->lexerErrors.push_back(lexerError);
            parser->lexerErrors.push_back(parser->lexer->currentLexerError);
            parser->lexer->reseterrorOccured();
        }
    } while (!(parser->lexer->reachedEOF && ((parser->lexer->currPtr >= parser->lexer->bufferLen))));

    // You have all the details to run LL(1) Parser

    unsigned int tokenIndex = 0;
    parser->parseStack.push_back(parser->grammarRules[0].lhs);
    ppp X = parser->parseStack.back();
    ppp a = parser->parserTokenList[tokenIndex]->lexerValueTokenTypeIdentifier;
    printStackProductions(parser->parseStack);
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
                // error()
                // ok case
                cout << "Some GAY Terminal Error" << endl;
            }
        } else if (parser->parseTable[X][a].empty()) {
            // error()
            cout << endl
                 << "-------------------TESTING-------------------" << endl;
            for (auto it : parser->parseTable[X]) {
                ppp f = it.first;
                if (f.first == ParserTokenType::TERMINAL)
                    cout << terminal_names[f.second.terminal] << " ";
                else
                    cout << non_terminal_names[f.second.nonTerminal] << " ";
            }
            cout << "Table Input is Empty" << endl;
        } else if (!parser->parseTable[X][a].empty()) {
            // output the production
            parser->parseStack.pop_back();
            vector<ppp> productions = parser->parseTable[X][a][0].rhs;
            reverse(productions.begin(), productions.end());
            parser->parseStack.insert(parser->parseStack.end(), productions.begin(), productions.end());
        }
        X = parser->parseStack.back();
        printStackProductions(parser->parseStack);
    }
    //while (!(X == _END_MARKER));
}

// pair<string, string> _splitRule(string s) {
// }

void productionsRulesInit(Parser* parser) {
    // Assuming filename is always `rules.bnf`
    try {
        parser->rules_file.open("./src/parse_table/final_rules.bnf", fstream::in);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    string ruleBuffer;
    getline(parser->rules_file, ruleBuffer);
    while (!(parser->rules_file.eof())) {
        // cout << ruleBuffer << endl;
        if (ruleBuffer != "") {
            size_t pos = ruleBuffer.find(" -> ");
            if (pos != string::npos) {
                string lhs = ruleBuffer.substr(0, pos);
                string rhs = ruleBuffer.substr(pos + 4);

                // Debug print line
                // cout << lhs << " -> " << rhs << endl;
                ParserTypeValue lhsValue(TERMINALS::NONE_TERMINAL, convertStringToNT(lhs));
                // lhsValue.terminal = NONE_TERMINAL;
                // lhsValue.nonTerminal = convertStringToNT(lhs);
                ProductionRule pRule = ProductionRule(make_pair(ParserTokenType::NON_TERMINAL, lhsValue));
                if (rhs == "\'\'") {
                    // EMPTY NT
                    // ParserTypeValue value;
                    // value.terminal = TERMINALS::EMPTY;
                    // value.nonTerminal = NONE_NON_TERMINAL;
                    pRule.rhs.push_back(make_pair(ParserTokenType::TERMINAL, ParserTypeValue(TERMINALS::EMPTY, NON_TERMINALS::NONE_NON_TERMINAL)));
                } else {
                    vector<string> vs = split(rhs, " ");

                    // Create a production rule object for the current production rule.
                    for (string& s : vs) {
                        ParserTypeValue value;
                        if (isElement(s, terminal_names)) {
                            // value.terminal = ;
                            // value.nonTerminal = NONE_NON_TERMINAL;
                            pRule.rhs.push_back(make_pair(ParserTokenType::TERMINAL, ParserTypeValue(convertStringToT(s), NON_TERMINALS::NONE_NON_TERMINAL)));
                        } else if (isElement(s, non_terminal_names)) {
                            // value.terminal = NONE_TERMINAL;
                            // value.nonTerminal = convertStringToNT(s);
                            pRule.rhs.push_back(make_pair(ParserTokenType::NON_TERMINAL, ParserTypeValue(TERMINALS::NONE_TERMINAL, convertStringToNT(s))));
                        }
                    }
                }

                // Store the current rule with all rules.
                parser->grammarRules.push_back(pRule);

                // Space separate the rhs words.
                // check each workd (string comparision) and push_back into `rhs` member variable of production rule.
            } else {
                // Error case: Invalid Grammar
                cout << "[ERROR]: Invalid Grammar" << endl;
            }
        }
        getline(parser->rules_file, ruleBuffer);
    }

    // Read from file and store it.
}

ParserNode* determineParserNode(Token* lexerToken) {
    ParserTokenType nodeType = ParserTokenType::TERMINAL;
    ParserTypeValue terminalType(TERMINALS::NONE_TERMINAL, NON_TERMINALS::NONE_NON_TERMINAL);
    // terminalType.nonTerminal = NONE_NON_TERMINAL;
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
        case TOKEN_TYPE::LOGICAL_NOT: {
            terminalType.terminal = TERMINALS::NOT_OPERATOR;
            break;
        }
        case TOKEN_TYPE::LOGICAL_AND: {
            terminalType.terminal = TERMINALS::AND_OPERATOR;
            break;
        }
        case TOKEN_TYPE::LOGICAL_OR: {
            terminalType.terminal = TERMINALS::OR_OPERATOR;
            break;
        }
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
        case TOKEN_TYPE::LEFT_SQUARE_BR: {
            terminalType.terminal = TERMINALS::LEFT_SQUARE_BRACKET;
            break;
        }
        case TOKEN_TYPE::RIGHT_SQUARE_BR: {
            terminalType.terminal = TERMINALS::RIGHT_SQUARE_BRACKET;
            break;
        }
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
            }
            //  else if (lexerToken->lexeme == "float") {
            //     terminalType.terminal = TERMINALS::FLOAT_KEYWORD;
            // } else if (lexerToken->lexeme == "for") {
            //     terminalType.terminal = TERMINALS::FOR_KEYWORD;
            // }
            else if (lexerToken->lexeme == "function") {
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
            break;
        }
        default: {
            // Error case
        }
    }
    assert(terminalType.terminal != TERMINALS::NONE_TERMINAL);
    return new ParserNode(make_pair(nodeType, terminalType), lexerToken);
}

bool isElement(string element, vector<string> arr) {
    return find(arr.begin(), arr.end(), element) != arr.end();
}

TERMINALS convertStringToT(string s) {
    // find(terminal_names.begin(), terminal_names.end(), s); -> Not working: Complain to Bjarne Mama
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
    // map<pair<ParserTokenType, ParserTypeValue>, vector<pair<ParserTokenType, ParserTypeValue>>>
    bool notDone;
    unsigned int count = 0;

    for (unsigned int i = 0; i < terminal_names.size(); ++i) {
        // cout << terminal_names[i] << endl;
        // ParserTypeValue keyValue;
        // keyValue.terminal = convertStringToT(terminal_names[i]);
        // keyValue.nonTerminal = NONE_NON_TERMINAL;

        ppp p = make_pair(ParserTokenType::TERMINAL, ParserTypeValue(convertStringToT(terminal_names[i]), NON_TERMINALS::NONE_NON_TERMINAL));
        parser->firstSet[p] = {p};
    }

    do {
        notDone = false;

        for (unsigned int i = 0; i < parser->grammarRules.size(); ++i) {
            // vs rule = split(rules[i], "->");
            // if (rule.size() < 2) {
            //     cout << "KOTHI error";
            //     continue;
            // }
            // string nonterminal = rule[0];
            // vs development = split(rule[1], " ");
            // Already created grammarRules //

            // vs nonterminalFirsts = firsts[nonterminal];

            // parser->firstSet[parser->grammarRules[i].lhs] = vector<ppp>();
            vector<ppp> nonterminalFirsts = parser->firstSet[parser->grammarRules[i].lhs];

            vector<ppp> development = parser->grammarRules[i].rhs;

            // ParserTypeValue emptyValue;
            // emptyValue.terminal = TERMINALS::EMPTY;
            // emptyValue.nonTerminal = NON_TERMINALS::NONE_NON_TERMINAL;
            // ppp EPSILON = make_pair(ParserTokenType::TERMINAL, emptyValue);

            // if (development.size() == 1 && development[0] == EPSILON) {
            if (development.size() == 1 && development[0] == EPSILON) {
                notDone |= addUnique(EPSILON, nonterminalFirsts);
            } else {
                notDone |= collectFirsts4(parser->firstSet, development, nonterminalFirsts);
            }

            parser->firstSet[parser->grammarRules[i].lhs] = nonterminalFirsts;

            // for (auto it : parser->firstSet) {
            //     ppp key = it.first;
            //     if (key.first == ParserTokenType::TERMINAL) {
            //         cout << terminal_names[key.second.terminal ] << endl;
            //     } else {
            //         cout << non_terminal_names[key.second.nonTerminal ] << endl;
            //     }
            // }
            // cout << endl;
        }
        // printFirstTable(parser);
        // cout << endl
        //      << endl;
        ++count;
        // cout << "Done with all production rules: " << count << " times." << endl;
    } while (notDone);
}

bool isPPPElement(ppp element, vector<ppp>& rhs) {
    return (find(rhs.begin(), rhs.end(), element) != rhs.end());
}

bool addUnique(ppp element, vector<ppp>& arr) {
    if (!isPPPElement(element, arr)) {
        arr.push_back(element);
        return true;
    }
    return false;
}

bool collectFirsts4(map<ppp, vector<ppp>>& firsts, vector<ppp>& development, vector<ppp>& nonterminalFirsts) {
    // ParserTypeValue emptyValue;
    // emptyValue.terminal = TERMINALS::EMPTY;
    // emptyValue.nonTerminal = NON_TERMINALS::NONE_NON_TERMINAL;
    // ppp EPSILON = make_pair(ParserTokenType::TERMINAL, emptyValue);

    bool result = false;
    bool epsilonInSymbolFirsts = true;

    for (auto const& j : development) {
        ppp symbol = j;
        epsilonInSymbolFirsts = false;

        // Test with raw terminals //
        // if (isElement(symbol, terminals)) {
        //     result |= addUnique(symbol, nonterminalFirsts);
        //     break;
        // }
        if (symbol.first == ParserTokenType::TERMINAL) {
            // string terminal = terminal_names[symbol.second.terminal ];
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

// // Choose auto x when you want to work with copies.
// #define foritc(i, v) for (auto i = v)
// // Choose auto &x when you want to work with original items and may modify them.
// #define foritm(i, v) for (auto& i = v)
// // Choose auto const &x when you want to work with original items and will not modify them.
// #define forit(i, v) for (auto const& i = v)
// 31657

void generateFollowSets(Parser* parser) {
    // ParserTypeValue emptyValue;
    // emptyValue.terminal = TERMINALS::EMPTY;
    // emptyValue.nonTerminal = NON_TERMINALS::NONE_NON_TERMINAL;
    // ppp EPSILON = make_pair(ParserTokenType::TERMINAL, emptyValue);

    // ParserTypeValue endMarkerValue;
    // endMarkerValue.terminal = TERMINALS::NONE_TERMINAL;
    // endMarkerValue.nonTerminal = NON_TERMINALS::END_MARKER;
    // ppp _END_MARKER = make_pair(ParserTokenType::NON_TERMINAL, endMarkerValue);

    bool notDone;

    do {
        notDone = false;

        // for(auto const& rule : parser->grammarRules){
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

                // if (isElement(symbol, nonterminals)) {
                if (symbol.first == ParserTokenType::NON_TERMINAL) {
                    vector<ppp> symbolFollows = parser->followSet[symbol];

                    // implement this
                    // vs afterSymbolFirsts = collectFirsts3(vs(development.begin()+j+1, development.end()));
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
    // ParserTypeValue emptyValue;
    // emptyValue.terminal = TERMINALS::EMPTY;
    // emptyValue.nonTerminal = NON_TERMINALS::NONE_NON_TERMINAL;
    // ppp EPSILON = make_pair(ParserTokenType::TERMINAL, emptyValue);

    vector<ppp> result;
    bool epsilonInSymbolFirsts = true;

    // for (auto j : sequence) {
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
    // ParserTypeValue emptyValue;
    // emptyValue.terminal = TERMINALS::EMPTY;
    // emptyValue.nonTerminal = NON_TERMINALS::NONE_NON_TERMINAL;
    // ppp EPSILON = make_pair(ParserTokenType::TERMINAL, emptyValue);

    for (unsigned int i = 0; i < parser->grammarRules.size(); ++i) {
        // vs rule = split(rules[i], "->");

        // if (rule.size() < 2) {
        //     cout << "KOTHI error";
        //     continue;
        // }

        ppp nonterminal = parser->grammarRules[i].lhs;
        // string tmp;
        // stringstream str_strm(rule[1]);
        vector<ppp> development = parser->grammarRules[i].rhs;
        // while (str_strm >> tmp) {
        //   development.push_back(tmp);
        // }

        vector<ppp> developmentFirsts = collectFirsts3(parser->firstSet, development);

        for (unsigned int j = 0; j < developmentFirsts.size(); ++j) {
            ppp symbol = developmentFirsts[j];

            if (symbol != EPSILON) {
                // parser->parseTable[nonterminal];
                vector<ProductionRule> oldTableRule = parser->parseTable[nonterminal][symbol];

                if (oldTableRule.empty()) {
                    parser->parseTable[nonterminal][symbol].push_back(parser->grammarRules[i]);  // = rules[i];
                } else {
                    parser->parseTable[nonterminal][symbol].push_back(parser->grammarRules[i]);  // oldTableRule + "\n" + rules[i];
                }
            } else {
                for (unsigned int j = 0; j < parser->followSet[nonterminal].size(); ++j) {
                    ppp symbol2 = parser->followSet[nonterminal][j];

                    // ruleTable[nonterminal];

                    vector<ProductionRule> oldTableRule = parser->parseTable[nonterminal][symbol2];

                    if (oldTableRule.empty()) {
                        parser->parseTable[nonterminal][symbol2].push_back(parser->grammarRules[i]);  // = rules[i];
                    } else {
                        parser->parseTable[nonterminal][symbol2].push_back(parser->grammarRules[i]);  // = oldTableRule + "\n" + rules[i];
                    }
                }
            }
        }
    }
}