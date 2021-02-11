# Design Constraints
## Language Constructs
- Sequential Statements
	- Declaration Statements
		- Typing: Static
			- Types: numeric: int, float, boolean
		- Naming:
			- Variable Naming: ""
			- Function Naming: ""
	- Assignment Statements
		- Binary: :=
	- Arithmetic Statements
		- Unary: ++, --
		- Binary: +, -, *, /, %
	- Logical Statements
		- Unary: !
		- Binary: &&, ||
	- Relational Statements
		- Binary: >, <, >=, <=, =/=, =
	- Bitwise Statements
		- Unary: ~
		- Binary: &, |, >>, <<
- Conditionals
	- If
	- If..Else
- Loops
	- For
	- While

## Precedence Order: Highest to Lowest
|    Op Name    |  Operator  |
|:-------------:|:----------:|
|Bitwise Not    |     ~      |
|Bitwise And    |     &      |
|Bitwise Or     |     \|     |
|Right Shift    |     >>     |
|Left Shift     |     <<     |
|Modulus        |     %      |
|Division       |     /      |
|Multiplication |     *      |
|Addition       |     +      |
|Subtraction    |     -      |
|Prefix Unary   |  ++ , --   | 
|Inequality     |<, >, <=, =>|
|Equality       |   =, =/=   |
|Logical Or     |    \|\|    |
|Logical And    |     &&     |
|Logical Not    |     !      |
|Comma          |     ,      |
|Array Indexing |     []     |
|Binding        |     ::     |
|Assignment     |     :=     |

## Token List
---------------------------------------------------------------------
|  TOKEN CODES  |         TOKEN TYPE        |   LEXEMES LIST / RE   |
|:-------------:|:-------------------------:|:---------------------:|
|     100       | Keywords 					| Test string 			|
|     101       | Assignment Operators		| :=					|
|     102       | Arithmetic Operators		| +, -, *, /, %, ++, --	|
|     103       | Logical Operators			| !, &&, \|\| 			|
|     105       | Relational Operators		| =, =/=, >, <, >=, <= 	|
|     108       | Bitwise Operators			| ~, &, \|, >>, <<		|
|     113       | Delimiters 				| (), {}, [], ;, , 		|
|     121       | Variable Identifiers		| [a-zA-Z]+(_[a-zA-Z]+)*|
|     134       | Function Identifiers		|      Test string      |
|     155       | Integer Literals 			| 0 \| [1-9][0-9]* \| -[1-9][0-9]* |
|     189       | Floating Point Literals 	| 0.0 \| "(+\|-)(0\|[1-9][0-9]\*).([0-9]\*[1-9])" |
---------------------------------------------------------------------

## Regular Expressions
| S |        Regular Expression         | F |
|:-:|:---------------------------------:|:-:|
| s | void								| f |
| s | boolean							| f |
| s | true								| f |
| s | false								| f |
| s | integer							| f |
| s | float								| f |
| s | if								| f |
| s | else								| f |
| s | for								| f |
| s | while								| f |
| s | end								| f |
| s | function							| f |
| s | end_definition					| f |
| s | @main								| f |
| s | break								| f |
| s | continue							| f |
| s | return							| f |
| s | NEWLINE							| f |
| s | INDENT							| f |
| s | 0\|[1-9][0-9]*					| f |
| s | 0.0 \| (0\|[1-9]).([0-9]*[1-9])?	| f |
| s | #.*								| f |