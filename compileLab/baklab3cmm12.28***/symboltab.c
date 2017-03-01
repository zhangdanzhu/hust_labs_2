#include "compiler.h"

/**
* @brief	初始化符号表
* @arg		无
* @retval	void
*/
void CreateSymTab()
{
	int i = 0;
	// 对于每个链表构造表头结点
	for (i = 0; i < MAX; i++)
	{
		SYMTAB[i] = CreateSymNode();
	}
	for (i = 0; i < SMAX; i++)
	{
		SYMS[i] = NULL;
	}
}

/**
* @brief	判断当前变量是否在符号表中
* @arg		name	:char*型，指向需要查找的变量名
* @arg		scale	:int型，表示查找范围，0为全部，1为当前作用域
* @arg		strkind	:int型，表示查找的是否为结构体声明，1为是
* @retval	VARTABLE *
* @return	return the node founded,if not found,return NULL
*/
symtable *FindSym(char *name, int scale, int strkind)
{
	symtable *temp = scale?(SYMS[SCOPE]):(SYMTAB[hash_pjw(name)]->next);
	while (temp != NULL)
	{
		if (!strcmp(temp->name, name) && temp->kind == strkind)
		{
			return temp;
		}
		else
		{
			temp = scale?(temp->snext):(temp->next);
		}
	}
	return temp;	
}

/**
* @brief	删除当前作用域
* @retval	void
*/
void DeleteScope()
{
	symtable *tempsym = SYMS[SCOPE];
	while (tempsym)
	{
		tempsym->pre->next = tempsym->next;
		if (tempsym->next)
		{
			tempsym->next->pre = tempsym->pre;
			tempsym->next = NULL;
		}
		tempsym->pre = NULL;
		SYMS[SCOPE] = tempsym->snext;
		tempsym->snext = NULL;
		free(tempsym);
		tempsym = SYMS[SCOPE];
	}
}

/**
* @brief	获取当前作用域符号链（用于struct和函数参数）
* @retval	symtable *
* @return	当前作用域链
*/
symtable *GetScope()
{
/*	symtable *tempscope = SYMS[SCOPE];
	symtable *tempsym = tempscope;
	while (tempsym != NULL)
	{
		// 从总符号表上断开
		tempsym->pre->next = tempsym->next;
		if (tempsym->next)
		{
			tempsym->next->pre = tempsym->pre;
			tempsym->next = NULL;
		}
		tempsym->pre = NULL;
		tempsym = tempsym->snext;
	}
	SYMS[SCOPE] = NULL;*/
	symtable *tempscope = NULL;
	symtable *tempsym = NULL;
	while (SYMS[SCOPE] != NULL)
	{
		tempsym = SYMS[SCOPE];
		// 从总符号表上断开
		tempsym->pre->next = tempsym->next;
		if (tempsym->next)
		{
			tempsym->next->pre = tempsym->pre;
			tempsym->next = NULL;
		}
		tempsym->pre = NULL;
		SYMS[SCOPE] = tempsym->snext;
		tempsym->snext = tempscope;
		tempscope = tempsym;
	}
	return tempscope;
}

/**
* @brief	处理ExtDef
* @arg		kind:	int型，用于选择使用的是哪条归约式
* @arg		gt1:	gramtree *型，第一个子（树）结点
* @arg		gt2:	gramtree *型，第二个子（树）结点，未用则为NULL
* @arg		gt3:	gramtree *型，第三个子（树）结点，未用则为NULL
* @retval	void	
*/ 
void DealExtDef(int kind, gramtree *gt1, gramtree *gt2, gramtree *gt3)
{
	symtable *temptype = NULL;
	symtable *tempsym = NULL;
	gramtree *tempvl = NULL;
	switch (kind)
	{
		case 0:
			// 处理specifier
			temptype = DealSpecifier(gt1);
			// 处理无意义声明
			break;
		case 1:
			// 处理specifier
			temptype = DealSpecifier(gt1);
			// 处理extdeclist
			if (temptype != NULL)
			{
				DealExtdl(gt2, temptype);
			}
			break;
		case 2:
			// 处理specifier
			temptype = DealSpecifier(gt1);
			if (temptype != NULL)
			{
				// 处理fundec
				if (FindSym(gt2->l_child->id_type_name, 1, 0) != NULL) // 重名
				{
					printf("Error type 4  at Line %d: Redefined function '%s'.\n", gt2->l_child->line, gt2->l_child->id_type_name);
					return;
				}
				else
				{
					// TODO
					// translateFunDec(gt2);

					tempsym = CreateSymNode();
					tempsym->type0 = 3;
					tempsym->name = gt2->l_child->id_type_name;
					tempsym->type = temptype;
					// 处理VarList
					tempvl = gt2->l_child->r_child->r_child;
					if (!strcmp(tempvl->lex_unit_name, "VarList"))
					{
						SCOPE++;
						sprintf(buf, "第%d层作用域：\n", SCOPE);
						fwrite(buf, strlen(buf), 1, fp);
						DealVarList(tempvl); // 处理VarList
						
						// TODO
						// translateVarList(tempvl);

						tempsym->args = GetScope();
						DealVarList(tempvl); // 将参数列表挂到函数结点后重新在符号表当前作用域挂上参数列表
					}
					else // 无形参
					{
						SCOPE++;
						sprintf(buf, "第%d层作用域：\n", SCOPE);
						fwrite(buf, strlen(buf), 1, fp);
						tempsym->args = NULL;
					}
				}
				// 处理compst
				DealCompSt(gt3, temptype);
				DeleteScope();
				SCOPE--;
				sprintf(buf, "第%d层作用域：\n", SCOPE);
				fwrite(buf, strlen(buf), 1, fp);
				InsertSym(tempsym);
			}
			break;
		default:
			return;
	}
}

/**
* @brief	处理CompSt
* @arg		compst:	gramtree *型，指向CompSt结点
* @arg		rettype：	symtable *型，指向返回值类型，用于比较RETURN类型
* @retval	void
*/
void DealCompSt(gramtree *compst, symtable *rettype)
{
	gramtree *tempsl = compst->l_child->r_child->r_child;
	DealDefList(compst->l_child->r_child, 0);
	while (tempsl->l_child != NULL) // 若有StmtList不产生空，则进行Stmt的处理
	{
		DealStmt(tempsl->l_child, rettype);
		tempsl = tempsl->l_child->r_child;
	}
}

/**
* @brief	处理Stmt
* @arg		stmt:	gramtree *类型，指向Stmt结点
* @arg		rettype:	symtable *类型，指向返回类型
* @retval	void
*/
void DealStmt(gramtree *stmt, symtable *rettype)
{
	gramtree *temp = stmt->l_child;
	if (!strcmp(temp->lex_unit_name, "Exp"))
	{
		DealExp(temp);		
	}
	else if (!strcmp(temp->lex_unit_name, "CompSt"))
	{
		DealCompSt(temp, rettype);
	}
	else if (!strcmp(temp->lex_unit_name, "RETURN"))
	{
		DealReturn(temp, rettype);
	}
	else if (!strcmp(temp->lex_unit_name, "IF"))
	{
		DealIf(temp, rettype);
	}
	else if (!strcmp(temp->lex_unit_name, "WHILE"))
	{
		DealWhile(temp, rettype);
	}
}

/**
* @brief	处理Exp,确定类型
* @arg		exp:	gramtree *型，指向Exp结点
* @retval	void
*/
void DealExp(gramtree *exp)
{
	gramtree *tempnode = exp->l_child;
	gramtree *tempnode2 = NULL;	
	char *tempname = tempnode->lex_unit_name;
	symtable *tempsym = NULL;
	// Exp...
	if (!(strcmp(tempname, "Exp")))
	{
		DealExp(tempnode);
		tempnode2 = tempnode->r_child;
		tempname = tempnode2->lex_unit_name;
		// 赋值语句
		if (!strcmp(tempname, "ASSIGNOP"))
		{
			if (!(tempnode->leftval)) // 不是左值
			{
				printf("Error type 6  at Line %d: The left-hand side of an assignment must be a variable.\n", tempnode->line);
			}
			else
			{
				DealExp(tempnode2->r_child);
				if (!CompType(tempnode->type, tempnode2->r_child->type))
				{
					printf("Error type 5  at Line %d: Type mismatched for assignment.\n", tempnode2->line);	
				}
				else
				{
					exp->type0 = tempnode->type0;
					exp->type = tempnode->type;
				}
			}
		}
		// 逻辑运算
		else if (!strcmp(tempname, "AND") || !strcmp(tempname, "OR"))
		{
			DealExp(tempnode2->r_child);
			if (tempnode->type0 != 0 || tempnode2->r_child->type0 != 0)
			{
				printf("Error type 7  at Line %d: Each operand's type must be 'int'.\n", tempnode2->line);
			}
			else
			{
				exp->type0 = 0;
				exp->type = tempnode->type;
			}
		}
		// 比较
		else if (!strcmp(tempname, "RELOP"))
		{
			DealExp(tempnode2->r_child);
			if ((tempnode->type0 != 0 && tempnode->type0 != 1) 
				|| (tempnode2->r_child->type0 != 0 && tempnode2->r_child->type0 != 1))
			{
				printf("Error type 7  at Line %d: Each operand's type must be 'int' or 'float'.\n", tempnode2->line);	
			}
			else if (!CompType(tempnode->type, tempnode2->r_child->type))
			{
				printf("Error type 7  at Line %d: Type mismatched for operands.\n", tempnode2->line);
			}
			else
			{
				exp->type0 = 0;
				exp->type = tempnode->type;
			}
		}
		// 算数运算
		else if (!strcmp(tempname, "PLUS") || !strcmp(tempname, "MINUS")
				|| !strcmp(tempname, "STAR") || !strcmp(tempname, "DIV"))
		{
			DealExp(tempnode2->r_child);
			if ((tempnode->type0 != 0 && tempnode->type0 != 1) 
				|| (tempnode2->r_child->type0 != 0 && tempnode2->r_child->type0 != 1))
			{
				printf("Error type 7  at Line %d: Each operand's type must be 'int' or 'float'.\n", tempnode2->line);	
			}
			else if (!CompType(tempnode->type, tempnode2->r_child->type))
			{
				printf("Error type 7  at Line %d: Type mismatched for operands.\n", tempnode2->line);
			}
			else
			{
				exp->type0 = tempnode->type0;
				exp->type = tempnode->type;
			}
		}
		// 数组
		else if (!strcmp(tempname, "LB"))
		{
			exp->leftval = 1;
			if (tempnode->type0 != 2)
			{
				printf("Error type 10 at Line %d: Illegal use of '[ ]'.\n", tempnode->line);	
			}
			else
			{
				DealExp(tempnode2->r_child);
				if (tempnode2->r_child->type0 != 0)
				{
					printf("Error type 12 at Line %d: Between the '[ ]' must be an integer.\n", tempnode2->r_child->line);
				}
				else
				{
					exp->type0 = tempnode->type->type->type0;
					exp->type = tempnode->type->type;
				}
			}
		}
		// 域
		else if (!strcmp(tempname, "DOT"))
		{
			exp->leftval = 1;
			if (tempnode->type->type0 != 4)
			{
				printf("Error type 13 at Line %d: Illegal use of '.'.\n", tempnode2->line);
			}
			else
			{
				tempname = tempnode2->r_child->id_type_name;
				tempsym = FindField(tempnode->type, tempname);  // 查找域
				if (tempsym == NULL)
				{
					printf("Error type 14 at Line %d: Non-existent field '%s'.\n", tempnode2->r_child->line, tempname);
				}
				else
				{
					exp->type0 = tempsym->type0;
					exp->type = tempsym->type;
				}
			}
		}
	}
	// (Exp)
	else if (!strcmp(tempname, "LP"))
	{
		DealExp(tempnode->r_child);
		exp->type0 = tempnode->r_child->type0;
		exp->type = tempnode->r_child->type;
	}
	// 取负
	else if (!strcmp(tempname, "MINUS"))
	{
		DealExp(tempnode->r_child);
		if (tempnode->r_child->type0 != 0 && tempnode->r_child->type0 != 1) // 既非int也非float
		{
			printf("Error type 7  at Line %d: The type following 'MINUS' must be 'int' or 'float'.\n", tempnode->line);
		}
		else
		{
			exp->type0 = tempnode->r_child->type0;
			exp->type = tempnode->r_child->type;
		}
	}
	// 逻辑非
	else if (!strcmp(tempname, "NOT"))
	{
		DealExp(tempnode->r_child);
		if (tempnode->r_child->type0 != 0) // 只有int型才能参与逻辑运算
		{
			printf("Error type 7  at Line %d: The type following 'NOT' must be 'int'.\n", tempnode->line);
		}
		else
		{
			exp->type0 = 0;
			exp->type = tempnode->r_child->type;
		}

	}
	// ID...
	else if (!strcmp(tempname, "ID"))
	{
		if (tempnode->r_child == NULL) // 仅有ID
		{
			exp->leftval = 1;
			tempsym = FindSym(tempnode->id_type_name, 0, 0);
			if (tempsym == NULL)
			{
				printf("Error type 1  at Line %d: Undefined variable '%s'.\n", tempnode->line, tempnode->id_type_name);
			}
			else
			{
				exp->type0 = tempsym->type0;
				exp->type = tempsym->type;
			}
		}
		else // 函数调用
		{
			// 从符号表找到函数并获取返回值类型
			tempsym = FindSym(tempnode->id_type_name, 0, 0);
			if (tempsym == NULL)
			{
				printf("Error type 2  at Line %d: Undefined function '%s'.\n", tempnode->line, tempnode->id_type_name);
			}
			else
			{
				if (tempsym->type0 != 3)
				{
					printf("Error type 11 at Line %d: '%s' is not a function.\n", tempnode->line, tempnode->id_type_name);
				}
				else
				{
					int argnum = 0;  // 记录实参为第几个
					exp->type0 = tempsym->type0;
					exp->type = tempsym->type;
					// 比较参数类型
					// 获取第一个实参与第一个形参
					tempnode2 = tempnode->r_child->r_child->l_child;
					argnum++;
					tempsym = tempsym->args;
					while (tempsym != NULL && tempnode2 != NULL)  // 直到形参或实参没有了才结束
					{
						DealExp(tempnode2);
						if (0 == CompType(tempsym->type, tempnode2->type))
						{
							printf("Error type 9 at Line %d: Type of %dth argument is mismatched, shoud be '%s'.\n", tempnode2->line, argnum, TransType(tempsym->type));
						}
						// 比较下一个参数
						tempsym = tempsym->snext;
						if (tempnode2->r_child != NULL)
						{
							tempnode2 = tempnode2->r_child->r_child->l_child;
							argnum++;
						}
						else
						{
							tempnode2 = NULL;
						}
					}
					if(tempsym != NULL) // 参数过少
					{
						printf("Error type 9  at Line %d: Miss arguments.\n", tempnode->line);
					}
					else if (tempnode2 != NULL) // 参数过多
					{
						printf("Error type 9  at Line %d: Too much arguments.\n", tempnode->line);
					}
				}
			}
			
		}
	}
	// int
	else if (!strcmp(tempname, "INT"))
	{
		exp->type0 = 0;
		exp->type = CreateSymNode();
		exp->type->type0 = 0;
	}
	// float
	else if (!strcmp(tempname, "FLOAT"))
	{
		exp->type0 = 1;
		exp->type = CreateSymNode();
		exp->type->type0 = 1;
	}
}

/**
* @brief	将type类型转换为字符串
* @arg		typenode:	symtable *类型，指向待转换类型
* @retval	char *
* @return	转换后的字符串
*/
char *TransType(symtable *typenode)
{
	char *result;
	switch (typenode->type0)
	{
		case 0:
			result = "int";
			break;
		case 1:
			result = "float";
			break;
		case 2:
			result = "array";
			break;
		case 4:
			if (typenode->name != NULL)
			{
				result = (char *)malloc(sizeof(typenode->name) + sizeof(char) * 8);
				strcpy(result, "struct ");
				strcat(result, typenode->name);
			}
			else 
			{
				result = "struct";
			}
	}
	return result;
}

/**
* @brief	查找域
* @arg		structsym:	symtable *类型，指向结构体
* @arg		fieldname:	char *类型，指向所要查找的域名
* @retval	symtable *
* @return	查找到的域
*/
symtable *FindField(symtable *structsym, char *fieldname)
{
	symtable *tempsym = structsym->type;
	while (tempsym != NULL)
	{
		if (!strcmp(tempsym->name, fieldname))
		{
			return tempsym;
		}
		else
		{
			tempsym = tempsym->snext;
		}
	}
	return tempsym;
}


/**
* @brief	处理RETURN
* @arg		retnode:	gramtree *类型，指向RETURN结点
* @arg		rettype:	symtable *类型，指向返回类型
* @retval	void
*/
void DealReturn(gramtree *retnode, symtable *rettype)
{
	DealExp(retnode->r_child);
	if (!CompType(rettype, retnode->r_child->type))
	{
		printf("Error type 8  at Line %d: Type mismatched for return.\n", retnode->line);
	}
}

/**
* @brief	处理IF
* @arg		ifnode:		gramtree *类型，指向IF结点
* @arg		rettype:	symtable *类型，指向返回类型
* @retval	void
*/
void DealIf(gramtree *ifnode, symtable *rettype)
{
	gramtree *temp = ifnode->r_child->r_child;
	DealExp(temp);
	if (temp->type0 != 0)
	{
		printf("Error type 19 at Line %d: Conditional expression's type must be 'int'.\n", temp->line);
	}
	temp = temp->r_child->r_child;
	SCOPE++;
	sprintf(buf, "第%d层作用域：\n", SCOPE);
	fwrite(buf, strlen(buf), 1, fp);
	DealStmt(temp, rettype);
	DeleteScope();
	SCOPE--;
	sprintf(buf, "第%d层作用域：\n", SCOPE);
	fwrite(buf, strlen(buf), 1, fp);
	if (temp->r_child != NULL) // 还有ELSE
	{
		temp = temp->r_child->r_child;
		SCOPE++;
		sprintf(buf, "第%d层作用域：\n", SCOPE);
		fwrite(buf, strlen(buf), 1, fp);
		DealStmt(temp, rettype);
		DeleteScope();
		SCOPE--;
		sprintf(buf, "第%d层作用域：\n", SCOPE);
		fwrite(buf, strlen(buf), 1, fp);
	}
}

/**
* @brief	处理WHILE
* @arg		whilenode:	gramtree *类型，指向IF结点
* @arg		rettype:	symtable *类型，指向返回类型
* @retval	void
*/
void DealWhile(gramtree *whilenode, symtable *rettype)
{
	gramtree *temp = whilenode->r_child->r_child;
	DealExp(temp);
	if (temp->type0 != 0)
	{
		printf("Error type 19 at Line %d: Conditional expression's type must be 'int'.\n", temp->line);
	}
	temp = temp->r_child->r_child;
	SCOPE++;
	sprintf(buf, "第%d层作用域：\n", SCOPE);
	fwrite(buf, strlen(buf), 1, fp);
	DealStmt(temp, rettype);
	DeleteScope();
	SCOPE--;
	sprintf(buf, "第%d层作用域：\n", SCOPE);
	fwrite(buf, strlen(buf), 1, fp);
}

/**
* @brief	处理VarList
* @arg		vl:		gramtree *型，指向VarList
* @retval	void
*/
void DealVarList(gramtree *vl)
{
	gramtree *temppd = vl->l_child;
	symtable *temptype = NULL;
	do {
		temptype = DealSpecifier(temppd->l_child);
		DealVarDec(temppd->l_child->r_child, temptype);
		if (temppd->r_child != NULL)
		{
			temppd = temppd->r_child->r_child->l_child;
		}
		else
		{
			temppd = NULL;
		}
	}while (temppd); // 当还有ParamDec的时候
}

/**
* @brief	处理ExtDecList
* @arg		gt2:	gramtree *型，指向ExtDecList
* @arg		temptype:	symtable *型，指向类型
* @retval	void
*/
void DealExtdl(gramtree *gt2, symtable *temptype)
{
	gramtree *tempvd = gt2->l_child;
	do {
		DealVarDec(tempvd, temptype);
		if (tempvd->r_child != NULL) // 还有ExtDecList
		{
			tempvd = tempvd->r_child->r_child->l_child;
		}
		else
		{
			tempvd = NULL;
		}
	}while (tempvd);
}

/**
* @brief	处理specifier
* @arg		spe:	gramtree *型，指向specifier
* @retval	symboltable *
* @return	返回类型
*/
symtable *DealSpecifier(gramtree *spe)
{
	gramtree *tempspe = spe->l_child;
	gramtree *tempopt = NULL;
	symtable *temptype = NULL;
	char *tempname;
	if (!strcmp(tempspe->lex_unit_name, "TYPE"))
	{
		temptype = CreateSymNode();
		if (!strcmp(tempspe->id_type_name, "int"))
		{
			temptype->type0 = 0;
		}
		else
		{
			temptype->type0 = 1;
		}
	}
	else  // 否则为StructSpecifier
	{
		tempname = tempspe->l_child->r_child->lex_unit_name;
		if (!strcmp(tempname, "Tag"))
		{
			tempname = tempspe->l_child->r_child->l_child->id_type_name; // ID名
			if ((temptype = FindSym(tempname, 0, 1)) == NULL) // 结构体类型未声明
			{
				printf("Error type 17 at Line %d: Undefined struct '%s'.\n", spe->line, tempname);
			}
		}
		else  // 否则为OptTag
		{
			tempopt = tempspe->l_child->r_child;
			if (tempopt->l_child == NULL) // OptTag为空
			{
				temptype = CreateSymNode();
				temptype->type0 = 4;
				SCOPE++;
				sprintf(buf, "第%d层作用域：\n", SCOPE);
				fwrite(buf, strlen(buf), 1, fp);
				temptype->type = DealDefList(tempopt->r_child->r_child, 1);  // 处理DefList
				SCOPE--;
				sprintf(buf, "第%d层作用域：\n", SCOPE);
				fwrite(buf, strlen(buf), 1, fp);
			}
			else
			{
				tempname = tempopt->l_child->id_type_name;
				if (FindSym(tempname, 1, 1) != NULL) // 重复声明
				{
					printf("Error type 16 at Line %d: Redefined struct '%s'.\n", spe->line, tempname);
					return NULL;
				}
				else
				{
					// 加入符号表
					temptype = CreateSymNode();
					temptype->type0 = 4;
					temptype->name = tempname;
					temptype->kind = 1;
					SCOPE++;
					sprintf(buf, "第%d层作用域：\n", SCOPE);
					fwrite(buf, strlen(buf), 1, fp);
					temptype->type = DealDefList(tempopt->r_child->r_child, 1);   // 处理DefList
					SCOPE--;
					sprintf(buf, "第%d层作用域：\n", SCOPE);
					fwrite(buf, strlen(buf), 1, fp);
					InsertSym(temptype); 
				}
			}
		}
	}
	return temptype;
}

/**
* @brief	处理DefList 
* @arg		dl:		gramtree *型，指向DefList结点
* @arg		stru:	int型，标记是否为struct内容,1是0否
* @retval	symtable *
* @return	返回一串符号，可用于挂在struct上
*/
symtable *DealDefList(gramtree *dl, int stru)
{
	symtable *templist = NULL;
	symtable *temptype = NULL;
	while (dl->l_child) // 还有Def
	{
		temptype = DealSpecifier(dl->l_child->l_child);
		DealDecList(dl->l_child->l_child->r_child, temptype, stru); // 处理DecList
		dl = dl->l_child->r_child;
	}
	// 若是结构体则将当前作用域符号链赋给templist
	if (stru)
	{
		templist = GetScope();
	}
	return templist;
}

/**
* @brief	处理DecList
* @arg		dl：	gramtree *型，指向DecList结点
* @arg		sptype:	symtable *型，指向类型 
* @arg		stru:	int型，标记是否为struct内容，1是0否(结构体中无赋值)
* @retval	void
*/
void DealDecList(gramtree *dl, symtable *sptype, int stru)
{
	gramtree *tempdec = dl->l_child;
	do {
		if (tempdec->l_child->r_child != NULL)
		{
			if (stru)
			{
				printf("Error type 15 at Line %d: assign in struct.\n", tempdec->line);
			}
			else
			{
				DealVarDec(tempdec->l_child, sptype);		
				DealExp(tempdec->l_child->r_child->r_child);
				if (!CompType(tempdec->l_child->type, tempdec->l_child->r_child->r_child->type)) //赋值号两边类型不同
				{
					printf("Error type 5 at Line %d: Type mismatched for assignment\n.", tempdec->l_child->r_child->line);
				}
			}
		}
		else
		{
			// 处理VarDec
			DealVarDec(tempdec->l_child, sptype);
		}
		if (tempdec->r_child != NULL) // 还有DecList
		{
			tempdec = tempdec->r_child->r_child->l_child;
		}
		else
		{
			tempdec = NULL;
		}
	}while (tempdec);
}

/**
* @brief	处理VarDec
* @arg		vd：	gramtree *型，指向VarDec结点
* @arg		sptype:	symtable *型，指向类型 
* @retval	void
*/
void DealVarDec(gramtree *vd, symtable *sptype)
{
	gramtree *tempvd = vd->l_child;
	symtable *tempsym = CreateSymNode();
	symtable *pretype = sptype;
	symtable *curtype = NULL;
	while (tempvd->r_child != NULL) // 没到ID
	{
		curtype = CreateSymNode();
		curtype->type0 = 2;
		curtype->size = tempvd->r_child->r_child->int_val;
		curtype->type = pretype;
		pretype = curtype;
		tempvd = tempvd->l_child;
	}
	// 到了ID之后
	if (FindSym(tempvd->id_type_name, 1, 0) != NULL)  // 重定义
	{
		printf("Error type 3  at Line %d: Redefined variable '%s'.\n", tempvd->line, tempvd->id_type_name);
	}
	else
	{
		tempsym->name = tempvd->id_type_name;
		tempsym->type0 = pretype->type0;
		tempsym->type = pretype;
		InsertSym(tempsym);
	}
	vd->type0 = tempsym->type0;
	vd->type = tempsym;
}

/**
* @brief	加入符号表
* @arg		newsym:		symtable *型，指向要加入符号表的符号
* @retval	void
*/
void InsertSym(symtable *newsym)
{
	newsym->next = SYMTAB[hash_pjw(newsym->name)]->next;
	newsym->pre = SYMTAB[hash_pjw(newsym->name)];
	SYMTAB[hash_pjw(newsym->name)]->next = newsym;
	if (newsym->next)
	{
		newsym->next->pre = newsym;
	}
	newsym->snext = SYMS[SCOPE];
	SYMS[SCOPE] = newsym;
	if (newsym->kind != 1)
	{
		sprintf(buf, "name: %s	type: %d\n", newsym->name, newsym->type0);
		fwrite(buf, strlen(buf), 1, fp);
	}
}

/**
* @brief	创建新符号表结点
* @arg		无
* @retval	symtable *
* @return	返回新创建的符号表结点
*/
symtable *CreateSymNode()
{
	symtable *temp = (symtable *)malloc(sizeof(symtable));
	temp->name = NULL;
	temp->type0 = -1;
	temp->size = 0;
	temp->kind = 0;
	temp->type = NULL;
	temp->next = NULL;
	temp->pre = NULL;
	temp->snext = NULL;
	return temp;
}

/** 
* @brief	hash查找
* @arg		name:	char *型，要查找的名字
* @retval	unsigned int
* @return	该名称变量所在位置
*/
unsigned int hash_pjw(char *name)
{
	unsigned val = 0, i;
	for ( ; *name; name++)
	{
		val = (val << 2) + *name;
		if (i = val & ~0x80)
		{
			val = (val ^ (1 >> 12)) & 0x80;
		}
		return val;
	}
}

/**
* @brief	比较两个类型是否相等
* @arg		type1:		symtable *型，指向要比较的第一个类型
* @arg		type2:		symtable *型，指向要比较的第二个类型
* @retval	int
* @return	若相等,返回1,否则返回0
*/
int CompType(symtable *type1, symtable *type2)
{
	symtable * tempfield1 = NULL;
	symtable * tempfield2 = NULL;
	if (NULL == type1 || NULL == type2 ) // 有一个为NULL则返回0
	{
		return 0;
	}
	else if (type1->type0 == type2->type0)
	{
		if (0 == type1->type0 || 1 == type1->type0)
		{
			return 1;
		}
		else if (2 == type1->type0) // 为数组类型
		{
			if (type1->size == type2->size)
			{
				return CompType(type1->type, type2->type);
			}
			else
			{
				return 0;
			}
		}
		else if (4 == type1->type0) // 为结构体类型
		{
			tempfield1 = type1->type;
			tempfield2 = type2->type;	
			while (tempfield1 != NULL && tempfield2 != NULL)
			{
				if (0 == CompType(tempfield1->type, tempfield2->type)) // 某个域不同
				{
					return 0;
				}
				else
				{
					tempfield1 = tempfield1->snext;
					tempfield2 = tempfield2->snext;
				}
			}
			if (tempfield1 == NULL && tempfield2 == NULL)
			{
				return 1;	
			}
			else
			{
				return 0;
			}
		}
	}
	else
	{
		return 0;
	}
}
