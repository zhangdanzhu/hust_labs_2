%{
	#include "syntax.tab.h"
	#include "gramtree.h"
	int yycolumn = 1;
	#define YY_USER_ACTION\
		yylloc.first_line = yylloc.last_line = yylineno;\
		yylloc.first_column = yycolumn;\
		yylloc.last_column = yycolumn + yyleng - 1;\
		yycolumn += yyleng;
%}

letter [_a-zA-Z]
relop >|<|>=|<=|==|!= 
octal (0([1-7][0-7]*))|0
decimal ([1-9][0-9]*)|0
hexadecimal (0x[1-9a-fA-F][0-9a-fA-F]*)|0
efloat ([0-9]*\.[0-9]+|[0-9]+\.)((E|e)[+-]?[0-9]+)?

%option yylineno

%x COMMENT

%%
int|float { 
	yylval.gt = Create_Node(yylineno, "TYPE", 0); 
	yylval.gt->id_type_name = (char *)malloc(sizeof(char) * strlen(yytext)); 
	strcpy(yylval.gt->id_type_name, yytext);
	return TYPE;
}
"struct" { yylval.gt = Create_Node(yylineno, "STRUCT", 0); return STRUCT;}
"return" { yylval.gt = Create_Node(yylineno, "RETURN", 0);  return RETURN;}
"if" { yylval.gt = Create_Node(yylineno, "IF", 0);  return IF;}
"else" { yylval.gt = Create_Node(yylineno, "TYPE", 0); return ELSE; }
"while" { yylval.gt = Create_Node(yylineno, "WHILE", 0); { return ELSE; }}
{letter}([0-9]|{letter}){0,31} { 
	yylval.gt = Create_Node(yylineno, "ID", 0);
	yylval.gt->id_type_name = (char *)malloc(sizeof(char) * strlen(yytext));
	strcpy(yylval.gt->id_type_name, yytext);
	return ID; 
}
{octal}|{decimal}|{hexadecimal} { 
	yylval.gt = Create_Node(yylineno, "INT", 0);
	yylval.gt->int_val = (int)strtol(yytext, NULL, 0);
	return INT;
}
{efloat} {
	yylval.gt = Create_Node(yylineno, "FLOAT", 0);
	yylval.gt->float_val = strtof(yytext, NULL);
	return FLOAT;
}
";" { yylval.gt = Create_Node(yylineno, "SEMI", 0);  return SEMI;} 
"," { yylval.gt = Create_Node(yylineno, "COMMA", 0);  return COMMA; }
"=" { yylval.gt = Create_Node(yylineno, "ASSIGNNOP", 0); return ASSIGNNOP; }
{relop} { yylval.gt = Create_Node(yylineno, "RELOP", 0); return RELOP; }
"+" { yylval.gt = Create_Node(yylineno, "PLUS", 0); return PLUS; }
"-" { yylval.gt = Create_Node(yylineno, "MINUS", 0); return MINUS; }
"*" { yylval.gt = Create_Node(yylineno, "STAR", 0);  return STAR; }
"/" { yylval.gt = Create_Node(yylineno, "DIV", 0); return DIV; }
"&&" { yylval.gt = Create_Node(yylineno, "AND", 0); return AND; }
"||" { yylval.gt = Create_Node(yylineno, "OR", 0); return OR; }
"." { yylval.gt = Create_Node(yylineno, "DOT", 0);  return DOT; }
"!" { yylval.gt = Create_Node(yylineno, "NOT", 0); return NOT; }
"(" { yylval.gt = Create_Node(yylineno, "LP", 0); return LP; }
")" { yylval.gt = Create_Node(yylineno, "RP", 0); return RP; }
"[" { yylval.gt = Create_Node(yylineno, "LB", 0); return LB; }
"]" { yylval.gt = Create_Node(yylineno, "RB", 0); return RB; }
"{" { yylval.gt = Create_Node(yylineno, "LC", 0); return LC; }
"}" { yylval.gt = Create_Node(yylineno, "RC", 0); return RC; }
\n { yycolumn = 1;}
[ \t] ;
"//" {
	char c = input();
	while (c != '\n') c = input();
}
"/*" { BEGIN COMMENT; /* 切换到注释模式 */ }
<COMMENT>. | 
<COMMENT>\n ; /* 抛弃注释文字 */
<COMMENT>"*/" { BEGIN INITIAL; /* 回到正常状态 */ }
0{letter}+ {
	printf("Error type A at Line %d: Mysterious characters \"%s\"\n", yylineno, yytext);
}

. {
	printf("Error type A at Line %d: Mysterious characters \"%s\"\n", yylineno, yytext);
}
%%
