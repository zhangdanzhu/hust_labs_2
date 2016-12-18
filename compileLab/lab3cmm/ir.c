#include "compiler.h"
#include "ir.h"

#define MAXSIZE 30

int labelNo; // 标签号
int varNo;   // 变量号

InterCodes *head = NULL;  // 中间代码链表头指针
InterCodes *tail = NULL;  // 中间代码链表尾指针

char *newLable()
{
	char *temp = (char *)malloc(sizeof(char) * MAXSIZE);
	sprintf(temp, "label%d", labelNo);
	labelNo++;
	return temp;
}

InterCode newInterCode()
{
	InterCode intercode = (InterCode)malloc(sizeof(struct InterCode));
	return intercode;
}

InterCodes *insertLink(InterCode intercode)
{
	InterCodes *newLink = (InterCodes *)malloc(sizeof(struct InterCodes));
	newLink->code = intercode;
	newLink->next = NULL;
	if (head == NULL)
	{
		newLink->prev = NULL;
		head = newLink;
		tail = newLink;

	}
	else
	{
		newLink->prev = tail;
		tail->next = newLink;
		tail = newLink;
	}
	return newLink;
}

Operand newOperand()
{
	Operand operand = (Operand)malloc(sizeof(struct Operand_));
	return operand;
}

/*void translateExtDef(gramtree *extdef) // 只有函数需要翻译
{
	translateFunDec(extdef->l_child->r_child);
	translateCompSt(extdef->l_child->r_child->r_child);
	return ;
}*/

void translateFunDec(gramtree *fundec)
{
	gramtree *tempnode = NULL;
	InterCode intercode = newInterCode(); 
	intercode->kind = FUNCTION;
	intercode->u.function.name = strdup(fundec->l_child->id_type_name);
	insertLink(intercode);				 
	return ;
}

void translateVarList(gramtree *varlist)
{
	gramtree *tempparam = varlist->l_child;
	translateParamDec(tempparam); // TODO
	while (tempparam->r_child != NULL)
	{
		tempparam = tempparam->r_child->r_child->l_child;
		translateParamDec(tempparam);
	}
	return ;
}

void translateParamDec(gramtree *paramdec)
{
	// TODO translateVarDec
	// char *id = translateVarDec(); 

	InterCode intercode = newInterCode();
	intercode->kind = PARAM;
	intercode->u.param.name = strdup(id);
}

/*
void translateCompSt(gramtree *compst)
{
	translateDefList(compst->l_child->r_child); // TODO
	translateStmtList(compst->l_child->r_child->r_child);
	return ;
}

void translateStmtList(gramtree *stmtlist)
{
	if (stmtlist->l_child != NULL)
	{
		translateStmt(stmtlist->l_child);
		translateStmtList(stmtlist->l_child->r_child);
	}
	return ;
}
*/
/*void translateStmt(gramtree *stmt)
{
	cahr *tempname = stmt->l_child->lex_unit_name;
	if (!strcmp(tempname, "Exp"))
	{
		translateExp(); // TODO
	}
	else if (!strcmp(tempname, "CompSt"))
	{
		translateCompSt();
	}
	else if (tempname, "RETURN")
	{
		// TODO
		Operand t1 = newVar();
		translate
	}
	else if ()
	{
				
	}
}*/

void translate_Exp(gramtree *expnode, Operand operand) 
{
	char *tempname = expnode->l_child->lex_unit_name;
	if (!strcmp(tempname, "Exp"))	
	{
		
	}
	else if (!strcmp(tempname, "LP"))
	{
		translateExp(expnode->l_child->r_child, operand);
	}
	else if (!strcmp(tempname, "MINUS"))
	{
		Operand t1 = newVar();
		translateExp(expnode->l_child->r_child, t1);
		
		InterCode intercode = newInterCode();
		intercode->kind = MINUS_;
		intercode->u.binop.result = operand;
		intercode->u.binop.operand1 = newOperand();
		intercode->u.binop.operand1->kind = CONSTANT;
		intercode->u.binop.operand1->u.value = 0;
		intercode->u.binop.operand2 = t1;

		insertLink(intercode);
	}
	else if (!strcmp(tempname, "NOT"))
	{
		char *label1 = newLabel();
		char *label2 = newLabel();

		InterCode intercode = newInterCode();
		intercode->kind =  
	}
	else if (!strcmp(tempname, "ID"))
	{

	}
	else if (!strcmp(tempname, "INT"))
	{

	}
	else if (!strcmp(tempname, "FLOAT"))
	{

	}
}

void outputFile(char *fileName)
{
	FILE* file = fopen(fileName, "w+");
	if (file != NULL)
	{
		InterCodes* tempcode = head;
		for (tempcode = head; tempcode != NULL; tempcode = tempcode->next)
		generateCode(file, tempcode->code);
	}
	else
	{
		printf("Fail to open file!\n");
		exit(-1);
	}
	fclose(file);
}

void generateCode(FILE *f, InterCode interCode)
{
	switch(interCode->kind)
	{
		case FUNCTION:
			fprintf(f, "FUNCTION %s :\n", interCode->u.function.name);
			break;
		/*case PARAM:
			fprintf(f, "PARAM %s\n", interCode->u.param.name);
			break;
		case LABEL:
			fprintf(f, "LABEL %s :\n", interCode->u.label.name);
			break;
		case GOTO_LABEL:
			fprintf(f, "GOTO %s\n", interCode->u.label_goto.name);
			break;
		case CALL:
			fprintf(f, "%s := CALL %s\n", interCode->u.call.operand->u.name, interCode->u.call.name);
			break;
		case RETURN_:
			if(interCode->u.ret.operand->kind == CONSTANT)
			{
				fprintf(f, "RETURN #%d\n", interCode->u.ret.operand->u.value)
			}
			else
			{
				fprintf(f, "RETURN %s\n", interCode->u.ret.operand->u.name);
			}
			break;
		case ARG:
			if(interCode->u.arg.operand->kind==CONSTANT)
			{
				fprintf(f,"ARG #%d\n",interCode->u.arg.operand->u.value);
			}
			else
			{
				fprintf(f,"ARG %s\n",interCode->u.arg.operand->u.name);
			}
			break;
		case DEC:
			fprintf(f, "DEC %s %d\n", interCode->u.dec.operand->u.name, interCode->u.dec.size);
			break;
		case COND:
			if(interCode->u.cond.operand1->kind==CONSTANT&&interCode->u.cond.operand2->kind==CONSTANT)
			{
				fprintf(f,"IF #%d %s #%d GOTO %s\n",interCode->u.cond.operand1->u.value,interCode->u.cond.op,interCode->u.cond.operand2->u.value,interCode->u.cond.name);
			}
			else if(interCode->u.cond.operand1->kind == CONSTANT)
			{
				fprintf(f,"IF #%d %s %s GOTO %s\n",interCode->u.cond.operand1->u.value,interCode->u.cond.op,interCode->u.cond.operand2->u.value,interCode->u.cond.name);
			}
			else if(interCode->u.cond.operand2->kind == CONSTANT)
			{
				fprintf(f,"IF %s %s #%d GOTO %s\n",interCode->u.cond.operand1->u.value,interCode->u.cond.op,interCode->u.cond.operand2->u.value,interCode->u.cond.name);
			}
			else
			{
				fprintf(f,"IF %s %s %s GOTO %s\n", interCode->u.cond.operand1->u.value, interCode->u.cond.op,interCode->u.cond.operand2->u.value, interCode->u.cond.name);
			}
			break;
		case MINUS_:
			if(interCode->u.triop.operand1->kind==CONSTANT&&interCode->u.triop.operand2->kind==CONSTANT)
			{
				fprintf(f,"%s := #%d - #%d\n", interCode->u.triop.result->u.name, interCode->u.triop.operand1->u.value, interCode->u.triop.operand2->u.value);
			}
			else if(interCode->u.triop.operand1->kind==CONSTANT)
			{
				fprintf(f,"%s := #%d - %s\n",interCode->u.triop.result->u.name, interCode->u.triop.operand1->u.value, interCode->u.triop.operand2->u.name);	
			}
			else if(interCode->u.triop.operand2->kind==CONSTANT)
			{
				fprintf(f,"%s := %s - %d\n", interCode->u.triop.result->u.name, interCode->u.triop.operand1->u.name, interCode->u.triop.operand2->u.value); 
			}
			else
			{
				fprintf(f,"%s := %s - %s\n", interCode->u.triop.result->u.name, interCode->u.triop.operand1->u.name, interCode->u.triop.operand2->u.name);
			}
			break;
		case ADD_:
			if(interCode->u.triop.operand1->kind==CONSTANT&&interCode->u.triop.operand2->kind==CONSTANT)
			{
				fprintf(f,"%s := #%d + #%d\n", interCode->u.triop.result->u.name, interCode->u.triop.operand1->u.value,interCode->u.triop.operand2->u.value);
			}
			else if(interCode->u.triop.operand1->kind==CONSTANT)
			{
				fprintf(f,"%s := #%d + %s\n", interCode->u.triop.result->u.name, interCode->u.triop.operand1->u.value, interCode->u.triop.operand2->u.name);
			}
			else if(interCode->u.triop.operand2->kind==CONSTANT)
			{
				fprintf(f,"%s := %s + #%d\n", interCode->u.triop.result->u.name, interCode->u.triop.operand1->u.name,interCode->u.triop.operand2->u.value);
			}
			else
			{
				fprintf(f,"%s := %s + %s\n", interCode->u.triop.result->u.name, interCode->u.triop.operand1->u.name, interCode->u.triop.operand2->u.name);
			}
			break;
		case SUB_:*/
		default:
			printf("Error!");
			break;
	}
}


