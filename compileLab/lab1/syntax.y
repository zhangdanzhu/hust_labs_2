%{
	#include <stdio.h>
	#include "lex.yy.c"
%}

%union{
	int
}

%token SEMI COMMA ASSIGNNOP RELOP
%token PLUS MINUS STAR DIV
%token AND OR DOT NOT 
%token LP RP LB RB LC RC

%right ASSIGNOP NOT
%left AND OR
%left RELOP
%left PLUS MINUS
%left STAR DIV 
%left LP RP LB RB DOT

