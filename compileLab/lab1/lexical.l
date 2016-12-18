%{
	#include "syntax.tab.h"
%}

digit [0-9]
letter [_a-zA-Z]
sign >|<|>=|<=|==|!= 
octal (0([1-7][0-7]*))|0
decimal ([1-9]{digit}*)|0
hexadecimal (0x[1-9a-fA-F][0-9a-fA-F]*)|0

%option yylineno

%%
("int")|("float") { return TYPE;}
"struct" { return STRUCT;}
"return" { return RETURN; }
"if" { return IF; }
"else" { return ELSE; }
"while" { return WHILE\n; }
{letter}({digit}|{letter}){0,31} { return ID; }
{octal}|{decimal}|{hexadecimal} { return INT; }
";" { return SEMI; }
"," { return COMMA; }
"=" { return ASSIGNNOP; }
{sign} { return RELOP; }
"+" { return PLUS; }
"-" { return MINUS; }
"*" { return STAR; }
"/" { return DIV; }
"&&" { return AND; }
"||" { return OR; }
"." { return DOT; }
"!" { return NOT; }
"(" { return LP; }
")" { return RP; }
"[" { return LB; }
"]" { return RB; }
"{" { return LC; }
"}" { return RC; }
\n {}
"//" {
	char c = input();
	while (c != '\n') c = input();
}

0{letter}+ {
	printf("Error type A at Line %d: %s\n", yylineno, yytext);
}
" " {
	;
}
. {
	printf("Error type A at Line %d: %s\n", yylineno, yytext);
}
%%
