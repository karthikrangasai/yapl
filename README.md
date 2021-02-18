# YAPL - Yet Another Programming Language

## Language Description
- **KEYWORDS**: void, boolean, true, false, integer, float, if, else, for, while, end, function, end_definition, @main, break, continue, return
- **OPERATORS**: ~, &, |, %, /, *, +, -, =, =/=, <=, >=, <, >, !, &&, ||, ::, :=
- **DELIMITERS**: [], {}, (), ;, ,
- **IDENTIFIERS**:
	- **VARIABLE IDENTIFIERS**: [a-zA-Z][_a-zA-Z0-9]*
	- **FUNCTION IDENTIFIERS**: @[a-zA-Z][_a-zA-Z0-9]*
- **INTEGER LITERALS**: 0 | [1-9][0-9]* 
- **FLOAT LITERALS**: ( 0 | [1-9][0-9]* ).( 0 | ([0-9]*[1-9]) )
- **STRING LITERALS**: "(.|\n)*"
