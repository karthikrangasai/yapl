# DFA Building
## BNF
### 
- [x] Program
	- [x] Functiions
		- [x] Function
		- [x] Main
	- [x] Statements
		- [x] Conditionals
		- [x] Loops
		- [x] Declaration Statements
		- [x] Arithmetic Statements
		- [x] Assignment Statements
	- [x] Number
	- [x] Boolean
	- [x] Identifier
- [ ] Remove resursion in grammar

### Testing
[BNF Testing Site](https://mdkrajnak.github.io/ebnftest/)

- [ ] Initialization
- [ ] Expressions
- [ ] Conditonals
- [ ] Loops
- [ ] Functions
- [ ] Test functions

# Coding
- [x] Define keywords array
- [x] Token enum
- [ ] main.c
	- [x] Accept File
	- [ ] Maybe Check extension
	- [x] Load the parser.c and call the parser function
- [ ] Parser
	- [x] Load lexer
	- [x] Run lexer when non EOF
	- [ ] Phase 2 of assignment
- [ ] Lexer
	- [x] Load lexer state
	- [x] Variable Indentifier
	- [x] Function Identifier
	- [x] Integer Literal Identifier
	- [x] Float Literal Identifier
	- [x] Operator Identifier
	- [x] Skip Empty lines
	- [x] Skip Comments
	- [x] Change while loop to loop on SEARCHING state.
	- [x] Add string data type
	- [x] Fix String RE : Gives error when "Hello worl\d"
	- [ ] O(n^2) -> O(n)
- [x] Token
	- [x] Define tokens
	- [x] functions for operators
	- [x] functions for variable identifier
	- [x] functions for function identifier
- [ ] Error Correction of Code

## Checking
In theory we have a clean separation between lexer and parser: Lexer splits a text into a sequence of tokens and then the parser constructs a tree out of the tokens according to the grammar. Each component should throw an error if they are not able to complete its task. So a lexer should throw an error if it cannot produce a valid token (e.g. if there is an unclosed string, or a sequence of characters which is not a valid token), while grammatical errors from valid tokens should be detected by the parser.

- Lexer errors:
	- Incomplete Float. Eg: (0|[1-9][0-9]*)".", 
	- Incorrect integers. Eg: 0[1-9]+, 
	- Seeing the characters: [`$^\:"'?]
	- Not matching double quotes for strings or invalid excape sequences

((0|[1-9][0-9]*).0) | ((0|[1-9][0-9]*).([0-9]*[1-9]))

(0|[1-9][0-9]*).(0|([0-9]*[1-9]))

## References
- Check indentation
	- References:
		- [Reddit Answer](https://www.reddit.com/r/ProgrammingLanguages/comments/7gvdje/any_advice_on_how_to_implement_the_pythonindent/)
		- [Python Way](https://docs.python.org/3/reference/lexical_analysis.html#lexical-analysis)
		- [Blog Post](https://jayconrod.com/posts/101/how-python-parses-white-space)

# Test Programs and Outputs
## Writing Examples
Check `./tests/examples/` directory for the examples.

- [x] Simple Example
- [ ] Initialization Example
- [ ] Conditonals Example
- [ ] Loops Example
- [x] Function Example
- [x] Recusrion Example
- [ ] Kirk Patrick Seidel Example