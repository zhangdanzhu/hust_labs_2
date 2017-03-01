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

Operand newVar()
{
    Operand temp = (Operand)malloc(sizeof(Operand_));
    temp->kind = VARIABLE;
    temp->u.name = (char *)malloc(MAXSIZE);
    sprintf(temp->u.name, "t%d", varNo);
    varNo += 1;
	
    return temp;
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
	char *id = translateVarDec(paramdec->l_child->r_child, 1);

	InterCode intercode = newInterCode();
	intercode->kind = PARAM;
	intercode->u.param.name = strdup(id);
	insertLink(intercode);
}

char *translateVarDec(gramtree *vd, int size)
{
	if (vd->l_child->l_child == NULL) // 为ID
	{
		char *id = vd->l_child->id_type_name;
		if (size > 4)
		{
			InterCode interCode = newInterCode();
			interCode->kind = DEC;
			interCode->u.dec.op = newOperand();
			interCode->u.dec.op->kind = ARRAY;
			interCode->u.dec.op->u.name = strdup(vd->l_child->id_type_name);
			interCode->u.dec.size = size;
			insertLink(interCode);
		}
		return id;
	}
	else
	{
		int n = vd->l_child->r_child->r_child->int_val;
		size *= n;
		return translateVarDec(vd->l_child, size);		
	}
}

void translateDec()

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

void translateExp(gramtree *exp, Operand operand)
{
	gramtree *node1 = exp->l_child;  // 最左孩子结点
	gramtree *node2 = node1->r_child; 
	char *tempname1 = node1->lex_unit_name;
	char *tempname2 = node2->lex_unit_name;
    if (!strcmp(tempname1, "Exp"))
    {
        if (!strcmp(tempname2, "ASSIGNOP"))
        {
            Unit it = get_unit(node->children[1]->value);			///***********************************************************************
            if (it == 0)
            {
                Operand t1 = newVar();

                translateExp(node2->r_child, t1);

                //Assign.
                InterCode interCode = newInterCode();
                interCode->kind = ASSIGN_;
                interCode->u.binop.result = newOperand();
                interCode->u.binop.result->kind = VARIABLE;
                interCode->u.binop.result->u.name = strdup(node->children[0]->children[0]->value);
                interCode->u.binop.operand = t1;
           
                insertLink(interCode);

            
                
                interCode = newInterCode();
               	interCode->kind = ASSIGN_;
                interCode->u.binop.result = operand;
                interCode->u.binop.operand = newOperand();
                interCode->u.binop.operand->kind = VARIABLE;
                   
                interCode->u.binop.operand->u.name = strdup(node->children[2]->children[0]->value);

                insertLink(interCode);
    
            }
            else if(it != 0 && it->kind == ARRAY_)
            {
                Operand* t1 = newVar();
                Operand* t2 = newVar();

                translateExp(node->children[0], t1);
                translateExp(node->children[2], t2);
                InterCode* interCode = newInterCode();
                interCode->kind = ASSIGN_;
                interCode->u.binop.result = t1;
                interCode->u.binop.operand = t2;
                insertLink(interCode);

                if (operand != 0)
                {
                    interCode = newInterCode();
                    interCode->kind = ASSIGN_;
                    interCode->u.binop.result = operand;
                    interCode->u.binop.operand = newOperand();
                    interCode->u.binop.operand->kind = VARIABLE;
                    interCode->u.binop.operand->u.name = strdup(node->children[2]->children[0]->value);
                }
            }
        }
        else if (strcmp(node->children[1]->name, "PLUS") == 0 ||
                 strcmp(node->children[1]->name, "MINUS") == 0 ||
                 strcmp(node->children[1]->name, "STAR") == 0 ||
                 strcmp(node->children[1]->name, "DIV") == 0)
        {
            Operand* t1 = newVar();
            Operand* t2 = newVar();

            translateExp(node->children[0], t1);
            translateExp(node->children[2], t2);

            InterCode* interCode;

            if (strcmp(node->children[1]->name, "PLUS") == 0)
            {
                interCode = newInterCode();
                interCode->kind = ADD_;
                interCode->u.triop.result = operand;
                interCode->u.triop.operand1 = t1;
                interCode->u.triop.operand2 = t2;
                insertLink(interCode);
            }
            else if (strcmp(node->children[1]->name, "MINUS") == 0)
            {
                interCode = newInterCode();
                interCode->kind = SUB_;
                interCode->u.triop.result = operand;
                interCode->u.triop.operand1 = t1;
                interCode->u.triop.operand2 = t2;
                insertLink(interCode);
            }
            else if (strcmp(node->children[1]->name, "STAR") == 0)
            {
                interCode = newInterCode();
                interCode->kind = MUL_;
                interCode->u.triop.result = operand;
                interCode->u.triop.operand1 = t1;
                interCode->u.triop.operand2 = t2;
                insertLink(interCode);
            }
            else
            {
                assert(strcmp(node->children[1]->name, "DIV") == 0);
                interCode = newInterCode();
                interCode->kind = DIV_;
                interCode->u.triop.result = operand;
                interCode->u.triop.operand1 = t1;
                interCode->u.triop.operand2 = t2;
                insertLink(interCode);
            }
        }
        else if ((strcmp(node->children[1]->name, "RELOP") == 0) ||
                 (strcmp(node->children[1]->name, "AND") == 0) ||
                 (strcmp(node->children[1]->name, "OR") == 0) ||
                 (strcmp(node->children[0]->name, "NOT") == 0))
        {

            string label1 = newLabel();
            string label2 = newLabel();

            InterCode* interCode = newInterCode();
            interCode->kind = ASSIGN_;
            interCode->u.binop.result = operand;
            interCode->u.binop.operand = newOperand();
            interCode->u.binop.operand->kind = CONSTANT;
            interCode->u.binop.operand->u.value = 0;

            insertLink(interCode);

            translateCond(node, label1, label2);

            interCode = newInterCode();
            interCode->kind = LABEL_;
            interCode->u.label.name = strdup(label1);
            insertLink(interCode);

            interCode = newInterCode();
            interCode->kind = ASSIGN_;
            interCode->u.binop.result = operand;
            interCode->u.binop.operand = newOperand();
            interCode->u.binop.operand->kind = CONSTANT;
            interCode->u.binop.operand->u.value = 1;

            insertLink(interCode);

            interCode = newInterCode();
            interCode->kind = LABEL_;
            interCode->u.label.name = strdup(label2);
            insertLink(interCode);

            free(label1);
            free(label2);

        }
        else if (strcmp(node->children[1]->name, "LB") == 0)
        {
			//EXP LB EXP RB
            Node* n = node->children[0]->children[0];

            for(; strcmp(n->name,"ID") != 0; n = n->children[0]);

            Unit it = get_unit(n->value);

            if (it == 0)
            {
                printf("Not Found.\n");
                return;
            }

            Type type = it->u.var->type;
            Type tp = type;
            for(; tp->u.array.elem->kind != BASIC ; tp = tp->u.array.elem);
            operand->kind = ADDRESS;
            translateArray(node, type, tp, 0, operand);

        }
    }
    else if (!strcmp(tempname1, "LP") == 0)
    {
        translateExp(node->children[1], operand);
    }
    else if (!strcmp(tempname1, "MINUS") == 0)
    {
        Operand* t1 = newVar();
        translateExp(node->children[1], t1);

        InterCode* interCode = newInterCode();
        interCode->kind = MINUS_;
        interCode->u.triop.result = operand;
        interCode->u.triop.operand1 = newOperand();
        interCode->u.triop.operand1->kind = CONSTANT;
        interCode->u.triop.operand1->u.value = 0;
        interCode->u.triop.operand2 = t1;
        insertLink(interCode);
    }
    else if (!strcmp(tempname1, "ID") == 0)
    {
        if (node->children_num == 1)
        {
            InterCode* interCode = newInterCode();
            interCode->kind = ASSIGN_;
            interCode->u.binop.result = operand;
            interCode->u.binop.operand = newOperand();
            interCode->u.binop.operand->kind = VARIABLE;
            interCode->u.binop.operand->u.name = strdup(node->children[0]->value);

            insertLink(interCode);
        }
        else if (node->children_num == 4)
        {
            //ID LP Args RP
            Operand* args[10];
            int i = 0;
            for (i = 0; i < 10; i++)
                args[i] = 0;

            translateArgs(node->children[2], args);

            //Deal with write.
            if (strcmp(node->children[0]->value, "write") == 0)
            {
                //printf("I'm writing...\n");
                InterCode* interCode = newInterCode();
                interCode->kind = WRITE_;
                interCode->u.write.operand = args[0];
                insertLink(interCode);
            }
            else
            {
                int i = 0;
                int j = 0;
                for (j = 0; args[j] != 0; j++);
                for (i = j - 1; i >= 0; i--)
                {
                    InterCode* interCode = newInterCode();
                    interCode->kind = ARG_;
                    interCode->u.arg.operand = args[i];
                    insertLink(interCode);
                }

                InterCode* interCode = newInterCode();
                interCode->kind = CALLFUNC_;
                interCode->u.call.operand = operand;
                interCode->u.call.name = strdup(node->children[0]->value);
                insertLink(interCode);
            }

        }
        else
        {
            //ID LP RP
            assert(node->children_num == 3);
            //Deal with read
            if (strcmp(node->children[0]->value, "read") == 0)
            {
                //printf("I'm reading...\n");
                InterCode* interCode = newInterCode();
                interCode->kind = READ_;
                interCode->u.read.operand = operand;
                insertLink(interCode);
            }
            else
            {
                InterCode* interCode = newInterCode();
                interCode->kind = CALLFUNC_;
                interCode->u.call.operand = operand;
                interCode->u.call.name = strdup(node->children[0]->value);
                insertLink(interCode);
            }
        }

    }
    else if(!strcmp(tempname1, "INT") == 0)
    {
        // printf("Should be here\n");
        operand->kind = CONSTANT;
        operand->u.value = strtol(node->children[0]->value, 0, 10);
        //printf("----%d\n", operand->u.value);
    }
    else
    {
        printf("%s\n", node->children[0]->name);
        // Do nothing.
    }
    // printf("exp over\n");
    return;

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
		case PARAM:
			fprintf(f, "PARAM %s\n", interCode->u.param.name);
			break;
		/*case LABEL:
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
			break;*/
		case DEC:
			fprintf(f, "DEC %s %d\n", interCode->u.dec.op->u.name, interCode->u.dec.size);
			break;
		/*case COND:
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


