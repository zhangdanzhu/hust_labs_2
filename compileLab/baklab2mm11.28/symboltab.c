#include "compiler.h"

void CreateSymTab()
{
	int i = 0;
	symtable *tempsym;
	// 对于每个链表构造表头结点
	for ( ; i < MAX; i++)
	{
		tempsym = (symtable *)malloc(sizeof(symtable));
		tempsym->name = NULL;
		tempsym->struct_name = NULL;
		tempsym->list = NULL;
		tempsym->slist = NULL;
		tempsym->next = NULL;
		tempsym->pre = NULL;
		tempsym->snext = NULL;
		SYMTAB[i] = tempsym;
	}
	for (i = 0; i < SMAX; i++)
	{
		SYMS[i] = NULL;
	}
}

/**
* @brief	判断当前变量是否在符号表中
* @arg		name	:char*型，指向需要查找的变量名
* @retval	VARTABLE *
* @return	return the node founded,if not found,return NULL
*/
symtable *FindSym(char *name)
{
	symtable *ph = SYMTAB[hash_pjw(name)]->next;
	while (ph != NULL)
	{
		if (strcmp(ph->name, name))
		{
			return ph;
		}
		else
		{
			ph = ph->next;
		}
	}
	return ph;	
}


int FindSymS(char *name)
{
	symtable *temp = SYMS[SCOPE];
	while (temp)
	{
		if (!strcmp(temp->name, name))
		{
			return 1;
		}
		else
		{
			temp = temp->snext;
		}
	}
	return 0;
}

void delete_scope()
{
	int i = 0;
	symtable *tempsym = SYMS[SCOPE];
	while (tempsym)
	{
		tempsym->pre->next = tempsym->next;
		if (tempsym->next)
		{
			tempsym->next->pre = tempsym->pre;
		}
		SYMS[SCOPE] = tempsym->snext;
		free(tempsym);
		tempsym = SYMS[SCOPE];
	}
}

void insertsym0(gramtree *gt1, gramtree *gt2)
{
	int kind = 0;
	char *str_name = NULL;
	gramtree *tempvd = gt2->l_child;
	gramtree *tempsp = gt1->l_child;
	if (!strcmp(tempsp->lex_unit_name, "TYPE")) // 变量类型TYPE
	{
		if (!strcmp(tempsp>id_type_name, "int")) // int型
		{
			kind = 0;
		}
		else // float型
		{
			kind = 1;
		}
	}
	else // 变量类型struct
	{
		tempsp = tempsp->l_child->r_child->l_child;
		if (tempsp)
		{
			str_name = tempsp->id_type_name;
		}
		// 若是变量重定义或未定义则提示
		// 保存下结构体的内容
		// 有名结构体定义则入符号表
		kind = 4;
	}

	while (tempvd)
	{
		if (!strcmp(tempvd->l_child->lex_unit_name, "VarDec")) // 该VarDec是数组的声明
		{
			insertvar(2, kind, tempvd->l_child, str_name);
		}
		else if (!strcmp(tempvd->l_child->lex_unit_name, "ID")) // 该VarDec是变量的声明
		{
			insertvar(kind, kind, tempvd->l_child, str_name);
		}
		tempvd = tempvd->r_child;
		if (!tempvd)
		{
			break;
		}
		// tempvd指向下一个vardec
		else
		{
			tempvd = tempvd->r_child->l_child;
		}
	}
}



void insertvar(int kind1, int kind2, gramtree *gt, char *str_name)
{
	dim_arg *templist = NULL;
	dim_arg *tempdim = NULL;
	int dimnum = 0;
	gramtree *tempgt = gt;
	gramtree *temp = NULL;
	while (strcmp(tempgt->lex_unit_name, "ID") != 0)  // 未到ID
	{
		// 维度增加
		dimnum++; 
		// 录入该维度长度
		temp = tempgt->r_child->r_child;
		tempdim = (dim_arg *)malloc(sizeof(dim_arg));
		tempdim->num = temp->int_val;
		tempdim->next = templist;
		templist = tempdim;
		tempgt = tempgt->l_child;
	}
	if (!FindSymS(tempgt->id_type_name)) // 当前作用域没找到该变量
	{
		symtable *tempsym = (symtable *)malloc(sizeof(symtable));		
		temsym->name = (char *)malloc(sizeof(char) * (strlen(tempgt->id_type_name) + 1));
		strcpy(tempsym->name, tempgt->id_type_name);
		tempsym->type = kind1;
		tempsym->type2 = kind2;
		if (str_name == NULL)
		{
			tempsym->struct_name = tempsym->name;
		}
		else
		{
			tempsym = (char *)malloc(sizeof(char) * (strlen(str_name) + 1));
			strcpy(tempsym->struct_name, str_name);
		}
		tempsym->dim_arg_num = dimnum;
		tempsym->list = templist;
		tempsym->snext = SYMS[SCOPE];
		SYMS[SCOPE] = tempsym;
		tempsym->next = SYMTAB[hash_pjw(tempsym->name)]->next;
		tempsym->pre = SYMTAB[hash_pjw(tempsym->name)];
		if (tempsym->next)
		{
			tempsym->next->pre = tempsym;
		}
	}
	else // 变量名重复
	{
		printf("Error type 3 at Line %d : 变量 %s 出现重复定义！\n", tempgt->line, tempgt->id_type_name);
	}
}


void insertstr0(gramtree *gt)
{
	gramtree *temp = gt->l_child->l_child;
	if (temp) // 为STRUCT
	{
		temp = temp->r_child;
		if (!strcmp(temp->lex_unit_name, "OptTag") && temp->l_child != NULL) 
		{
			temp = temp->l_child;
			if (!FindSymS(temp->id_type_name)) // 当前作用域无此变量
			{
				insertstr(temp);
			}
			else
			{
				printf("Error type 3 at Line %d : 变量 %s 出现重复定义！\n", temp->line, temp->id_type_name);
			}
		}
		else
		{
			printf("Error type 18 at line %d : 无意义的语句！",  temp->line);
		}
	}
	else
	{
		printf("Error type 18 at line %d : 无意义的语句！",  temp->line);
	}
}

void insertstr(gramtree *temp)
{
	str *tempslist = NULL;
	str *tempstrnod = NULL;
	gramtree *tempdef = NULL;
	symtable * tempsym = (symtable *)malloc(sizeof(symtable));
	tempsym->name = (char *)malloc(sizeof(char) * (strlen(temp->id_type_name) + 1));
	strcpy(tempsym->name, temp->id_type_name);
	tempsym->type = 4;
	tempsym->type2 = 4;
	tempsym->struct_name = tempsym->name;
	tempsym->dim_arg_num = 0;
	tempsym->list = NULL;
	tempsym->slist = NULL;		
	// TODO 获取结构体内容(结构体内部又是一层作用域)
	temp = temp->r_child->r_child; // DefList
	SCOPE++;
	while (temp->l_child) // Def
	{
		tempdef = temp->l_child;
		
		gramtree *tempspe = tempdef->l_child;
		gramtree *tempdl = tempdef->l_child->r_child;

		


		temp = temp->r_child // DefList
	}
	delete_scope();
	SCOPE--;

	tempsym->snext = SYMS[SCOPE];
	SYMS[SCOPE] = tempsym;
	tempsym->next = SYMTAB[hash_pjw(tempsym->name)]->next;
	tempsym->pre = SYMTAB[hash_pjw(tempsym->name)];
	if (tempsym->next)
	{
		tempsym->next->pre = tempsym;
	}
}


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
