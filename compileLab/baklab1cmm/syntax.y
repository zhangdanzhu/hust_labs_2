%{
	#include "gramtree.h"
	#include "lex.yy.c"
	void yyerror(char const *msg);
	int noerror = 1;
%}

%locations
%error-verbose

%union{
	gramtree* gt;
	int type_int;
}

%token <gt> TYPE
%token <gt> INT FLOAT
%token <gt> ID
%token <gt> IF ELSE WHILE RETURN STRUCT
%token <gt> SEMI COMMA ASSIGNNOP RELOP
%token <gt> PLUS MINUS STAR DIV
%token <gt> AND OR DOT NOT 
%token <gt> LP RP LB RB LC RC

%type <gt> Program ExtDefList ExtDef Specifier FunDec CompSt VarDec ExtDecList StructSpecifier OptTag DefList Tag VarList ParamDec StmtList Stmt Def DecList Dec Exp Args

%right ASSIGNNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV 
%right NOT
%left LP RP LB RB LC RC DOT

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%
/* High-level Definition */
Program : ExtDefList  { $$ = Create_Node(0, "Program", 1, $1); if (noerror) PreOrder_Traverse($$, 0); }
	;
ExtDefList : ExtDef ExtDefList { $$ = Create_Node(0, "ExtDefList", 2, $1, $2); }
	| /* empty */ {$$ = Create_Node(0, "ExtDefList", -1); }
	;
ExtDef : Specifier ExtDecList SEMI { $$ = Create_Node(0, "ExtDef", 3, $1, $2, $3); }
	| Specifier SEMI  { $$ = Create_Node(0, "ExtDef", 2, $1, $2); }
	| Specifier FunDec CompSt { $$ = Create_Node(0, "ExtDef", 3, $1, $2, $3); } 
	;
ExtDecList : VarDec { $$ = Create_Node(0, "ExtDecList", 1, $1); }
	| VarDec COMMA ExtDecList  { $$ = Create_Node(0, "ExtDecList", 3, $1, $2, $3); }
	;

/* Specifiers  */
Specifier : TYPE  { $$ = Create_Node(0, "Specifier", 1, $1); }
	| StructSpecifier { $$ = Create_Node(0, "Specifier", 1, $1); }
	;
StructSpecifier : STRUCT OptTag LC DefList RC   { $$ = Create_Node(0, "StructSpecifier", 5, $1, $2, $3, $4, $5); }
	| STRUCT Tag  { $$ = Create_Node(0, "StructSpecier", 2, $1, $2); }
	;
OptTag : ID { $$ = Create_Node(0, "OptTag", 1, $1); }
	| /* empty */  { $$ = Create_Node(0, "OptTag", -1); }
	;
Tag : ID  { $$ = Create_Node(0, "Tag", 1, $1); } 
	;

/* Declarators */
VarDec : ID  { $$ = Create_Node(0, "VarDec", 1, $1); }   
	| VarDec LB INT RB  { $$ = Create_Node(0, "VarDec", 4, $1, $2, $3, $4); } 
	;
FunDec : ID LP VarList RP  { $$ = Create_Node(0, "FunDec", 4, $1, $2, $3, $4); } 
	| ID LP RP  { $$ = Create_Node(0, "FunDec", 3, $1, $2, $3); }  
	;
VarList : ParamDec COMMA VarList  { $$ = Create_Node(0, "VarList", 3, $1, $2, $3); }  
	| ParamDec   { $$ = Create_Node(0, "VarList", 1, $1); } 
	;
ParamDec : Specifier VarDec  { $$ = Create_Node(0, "ParamDec", 2, $1, $2); } 
	;

/* Statements */
CompSt : LC DefList StmtList RC   { $$ = Create_Node(0, "CompSt", 4, $1, $2, $3, $4); } 
	;
StmtList : Stmt StmtList  { $$ = Create_Node(0, "StmtList", 2, $1, $2); } 
	| /* empty */  { $$ = Create_Node(0, "StmtList", -1); } 
	;
Stmt : Exp SEMI   { $$ = Create_Node(0, "Stmt", 2, $1, $2); } 
	| CompSt   { $$ = Create_Node(0, "Stmt", 1, $1); } 
	| RETURN Exp SEMI   { $$ = Create_Node(0, "Stmt", 3, $1, $2, $3); } 
	| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE  { $$ = Create_Node(0, "Stmt", 5, $1, $2, $3, $4, $5); } 
	| IF LP Exp RP Stmt ELSE Stmt  { $$ = Create_Node(0, "Stmt", 7, $1, $2, $3, $4, $5, $6, $7); } 
	| WHILE LP Exp RP Stmt  { $$ = Create_Node(0, "Stmt", 5, $1, $2, $3, $4, $5); } 
	;

/* Local Definition */
DefList : Def DefList  { $$ = Create_Node(0, "DefList", 2, $1, $2); } 
	| /* empty */  { $$ = Create_Node(0, "DefList", -1); } 
	;
Def : Specifier DecList SEMI  { $$ = Create_Node(0, "Def", 3, $1, $2, $3); } 
	| Specifier error SEMI  // 定义出错
	;
DecList: Dec   { $$ = Create_Node(0, "DecList", 1, $1); } 
	| Dec COMMA DecList  { $$ = Create_Node(0, "DecList", 3, $1, $2, $3); } 
	;
Dec: VarDec  { $$ = Create_Node(0, "Dec", 1, $1); } 
	| VarDec ASSIGNNOP Exp  { $$ = Create_Node(0, "Dec", 3, $1, $2, $3); } 
	;

/* Expressions */
Exp: Exp ASSIGNNOP Exp  { $$ = Create_Node(0, "Exp", 3, $1, $2, $3); } 
	| Exp AND Exp  { $$ = Create_Node(0, "Exp", 3, $1, $2, $3); } 
	| Exp OR Exp  { $$ = Create_Node(0, "Exp", 3, $1, $2, $3); } 
	| Exp RELOP Exp  { $$ = Create_Node(0, "Exp", 3, $1, $2, $3); } 
	| Exp PLUS Exp  { $$ = Create_Node(0, "Exp", 3, $1, $2, $3); } 
	| Exp MINUS Exp  { $$ = Create_Node(0, "Exp", 3, $1, $2, $3); } 
	| Exp STAR Exp  { $$ = Create_Node(0, "Exp", 3, $1, $2, $3); } 
	| Exp DIV Exp  { $$ = Create_Node(0, "Exp", 3, $1, $2, $3); } 
	| LP Exp RP  { $$ = Create_Node(0, "Exp", 3, $1, $2, $3); } 
	| MINUS Exp  { $$ = Create_Node(0, "Exp", 2, $1, $2); } 
	| NOT Exp  { $$ = Create_Node(0, "Exp", 2, $1, $2); } 
	| ID LP Args RP  { $$ = Create_Node(0, "Exp", 4, $1, $2, $3, $4); } 
	| ID LP RP  { $$ = Create_Node(0, "Exp", 3, $1, $2, $3); } 
	| Exp LB Exp RB  { $$ = Create_Node(0, "Exp", 4, $1, $2, $3, $4); } 
	| Exp DOT ID  { $$ = Create_Node(0, "Exp", 3, $1, $2, $3); } 
	| ID  { $$ = Create_Node(0, "Exp", 1, $1); } 
	| INT  { $$ = Create_Node(0, "Exp", 1, $1); } 
	| FLOAT  { $$ = Create_Node(0, "Exp", 1, $1); } 
	;
Args : Exp COMMA Args  { $$ = Create_Node(0, "Args", 3, $1, $2, $3); } 
	| Exp  { $$ = Create_Node(0, "Args", 1, $1); } 
	;
%%

void yyerror(char const *msg)
{
	noerror = 0;
	printf("Error Type B at Line %d, Column %d : '%s',  %s\n", yylineno, yycolumn, yytext, msg);
}
