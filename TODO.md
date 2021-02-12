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
- [ ] Lexer
	- [x] Load lexer state
	- [x] Variable Indentifier
	- [x] Function Identifier
	- [ ] Integer Literal Identifier
	- [ ] Float Literal Identifier
	- [x] Operator Identifier
	- [x] Skip Empty lines
	- [ ] Skip Comments: Doesn't work at all - Create a status flag or call reload buffer function (Need to create this function)
	- [ ] Check Indentation of blocks
- [ ] Token
	- [x] Define tokens
	- [x] functions for operators
	- [x] functions for variable identifier
	- [x] functions for function identifier

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
- [ ] Function Example
- [x] Recusrion Example
- [ ] Kirk Patrick Seidel Example