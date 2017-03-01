#ifndef IR_H
#define IR_H

#include "compiler.h"

struct Operand_
{
	enum { VARIABLE, CONSTANT, ADDRESS, ARRAY } kind;
	union
	{
		int var_no;
		int i_value;
		float f_value;
		char *name;
	}u;
};

typedef struct Operand_ *Operand;

struct InterCode
{
	enum {ASSIGN_, ADD_, SUB_, MUL_, DIV_, MINUS_, RETURN_, LABEL, GOTO_LABEL, CALL, FUNCTION, ARG, PARAM, DEC, COND} kind;
	union
	{
		struct {Operand right, left; } assign;	   
		struct {Operand result, op1, op2; } triop; 
		struct {Operand result, op; } binop;
		struct {Operand result;} sinop;	   	
		struct {Operand op; } ret;
		struct {char *name; } label;
		struct {char *name; } goto_label;
		struct {Operand op; char *name; } call;
		struct {char *name; } function;
		struct {Operand op; } arg;
		struct {char *name; } param;
		struct {Operand op; int size; } dec;
		struct {Operand op1, op2; char *op, *name; } cond;
	}u;
};

typedef struct InterCode *InterCode;

typedef struct InterCodes {
	InterCode code;
	struct InterCodes *prev, *next;
}InterCodes;


char *newLabel(); 
Operand newVar();
InterCode newInterCode();
InterCodes *insertLink(InterCode intercode);
void translateVarList(gramtree *varlist);
void translateParamDec(gramtree *paramdec);
char *translateVarDec(gramtree *vd, int size);
void translateExp(gramtree *exp, Operand operand);
void translateFunDec(gramtree *fundec);
void outputFile(char *filename);
void generateCode(FILE *f, InterCode interCode);
Operand newOperand();

#endif
