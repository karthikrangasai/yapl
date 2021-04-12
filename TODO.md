# DFA Building
## BNF
### Design
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
	- Lexer errors:
		- Incomplete Float. Eg: (0|[1-9][0-9]*)".", 
		- Incorrect integers. Eg: 0[1-9]+, 
		- Seeing the characters: [`$^\:"'?]
		- Not matching double quotes for strings or invalid excape sequences
- [x] Token
	- [x] Define tokens
	- [x] functions for operators
	- [x] functions for variable identifier
	- [x] functions for function identifier
- [ ] Error Correction of Code
### Test Programs and Outputs
#### Writing Examples
Check `./tests/examples/` directory for the examples.
- [x] Simple Example
- [x] Initialization Example
- [x] Conditonals Example
- [x] Loops Example
- [x] Function Example
- [x] Recursion Example
- [x] Complex example 1

# References
- Check indentation
	- References:
		- [Reddit Answer](https://www.reddit.com/r/ProgrammingLanguages/comments/7gvdje/any_advice_on_how_to_implement_the_pythonindent/)
		- [Python Way](https://docs.python.org/3/reference/lexical_analysis.html#lexical-analysis)
		- [Blog Post](https://jayconrod.com/posts/101/how-python-parses-white-space)



# Phase 2
- [ ] Add Assignment_Numeric_Expr for FOR LOOP
- [ ] Remove left and right shift operator
- [ ] Remove recursions

- [x] Add Prod rule reader
- [x] Add first set computing function
- [ ] Add follow set computing function
- [ ] Parse table generation
