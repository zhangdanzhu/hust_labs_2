#include "compiler.h"

void CreateSymTab()
{
	int i = 0;
	vartable *tempvar;
	functable *tempfunc;
	arraytable *temparray;
	structtable *STRUCTTAB;
	// 对于每个链表构造表头结点
	for ( ; i < 128; i++)
	{
		tempvar = (vartable *)malloc(sizeof(vartable));
		tempvar->name = NULL;
		tempvar->next = NULL;
		tempvar->pre = NULL;
		tempvar->lnext = NULL;
		tempfunc = (functable *)malloc(sizeof(functable));
		tempfunc->name = NULL;
		tempfunc->next = NULL;
		temparray = (arraytable *)malloc(sizeof(arraytable));
		temparray->name = NULL;
		temparray->list = NULL;
		temparray->next = NULL;
		temparray->pre = NULL;
		temparray->lnext = NULL;
		structtable = (structtable *)malloc(sizeof(structtable));
		structtable->name = NULL;
		structtable->next = NULL
		VARTAB[i] = tempvar;
		FUNCTAB[i] = tempfunc;
		ARRAYTAB[i] = temparray;
		STRUCTTAB[i] = tempstruct;
	}
	for (i = 0; i < 10; i++)
	{
		VARS[i] = NULL;
		ARRAYS[i] = NULL;
	}
}

/**
* @brief	判断当前变量是否在符号表中
* @arg		name	:char*型，指向需要查找的变量名
* @retval	VARTABLE *
* @return	return the node founded,if not found,return NULL
*/
vartable *FindVar(char *name)
{
	vartable *ph = VARTAB[hash_pjw(name)]->next;
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

/**
* @brief	判断当前函数是否在符号表中
* @arg		name	:char*型，指向需要查找的函数名
* @retval	functable *
* @return	return the node founded,if not found,return NULL
*/
functable *FindFunc(char *name)
{
	functable *ph = FUNCTAB[hash_pjw(name)]->next;
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

/**
* @brief	判断当前数组是否在符号表中
* @arg		name	:char*型，指向需要查找的数组名
* @retval	arraytable *
* @return	return the node founded,if not found,return NULL
*/
arraytable *FindArray(char *name)
{
	arraytable *ph = ARRAYTAB[hash_pjw(name)]->next;
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


/**
* @brief	判断当前结构体是否在符号表中
* @arg		name	:char*型，指向需要查找的结构体名
* @retval	structtable *
* @return	return the node founded,if not found,return NULL
*/
structtable *FindStruct(char *name)
{
	structtable *ph = STRUCTTAB[hash_pjw(name)]->next;
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


int FindVarS(char *name)
{
	vartable *temp = VARS[SCOPE];
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


int FindArrayS(char *name)
{
	arraytable *temp = ARRAYS[SCOPE];
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
	vartable *tempvar = VARS[SCOPE];
	arraytable *temparray = ARRAYS[SCOPE];
	while (tempvar)
	{
		tempvar->pre->next = tempvar->next;
		if (tempvar->next)
		{
			tempvar->next->pre = tempvar->pre;
		}
		VARS[SCOPE] = tempvar->snext;
		free(tempvar);
		tempvar = VARS[SCOPE];
	}
	while (temparray)
	{
		temparray->pre->next = temparray->next;
		if (temparray->next)
		{
			temparray->next->pre = temparray->pre;
		}
		ARRAYS[SCOPE] = temparray->snext;
		free(temparray);
		temparray = ARRAYS[SCOPE];
	}
}

void insertvars0(int kind, gramtree *gt)
{
	gramtree *tempvd = gt->l_child;
	while (1)
	{
		if (!strcmp(tempvd->l_child->lex_unit_name, "VarDec")) // 该VarDec是数组的声明
		{
			insertarray(kind, tempvd->l_child);
		}
		else if (!strcmp(tempvd->l_child->lex_unit_name, "ID")) // 该VarDec是变量的声明
		{
			insertvar(kind, tempvd->l_child);
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


void insertarray(int kind, gramtree *gt)
{
	dim *templist = NULL;
	dim *tempdim = NULL;
	int dimnum = 0;
	gramtree *tempgt = gt;
	gramtree *temp = NULL;
	while (strcmp(tempgt->lex_unit_name, "ID") != 0)  // 未到ID
	{
		// 维度增加
		dimnum++; 
		// 录入该维度长度
		temp = tempgt->r_child->r_child;
		tempdim = (dim *)malloc(sizeof(dim));
		tempdim->num = temp->int_val;
		tempdim->next = templist;
		templist = tempdim;
		tempgt = tempgt->l_child;
	}
	if (!FindVarS(tempgt->id_type_name) && !FindArrayS(tempgt->id_type_name)) // 当前作用域没找到该变量
	{
		if (!FindStruct(tempgt->id_type_name)) // 没找到同名结构体
		{
			arraytable *temparray = (arraytable *)malloc(sizeof(arraytable));		
			temparray->name = (char *)malloc(sizeof(char) * (strlen(tempgt->id_type_name) + 1));
			strcpy(temparray->name, tempgt->id_type_name);
			temparray->type = kind;
			temparray->dimension = dimnum;
			temparray->list = templist;
			temparray->snext = ARRAYS[SCOPE];
			ARRAYS[SCOPE] = temparray;
			temparray->next = ARRAYTAB[hash_pjw(temparray->name)]->next;
			temparray->pre = ARRAYTAB[hash_pjw(temparray->name)];
			if (temparray->next)
			{
				temparray->next->pre = temparray;
			}
		}
		else // 重名结构体
		{
			printf("Error type 3 at Line %d : 变量 %s 与结构体重名！", tempgt->line, tempgt->id_type_name);
		}
	}
	else // 变量名重复
	{
		printf("Error type 3 at Line %d : 变量 %s 出现重复定义！\n", tempgt->line, tempgt->id_type_name);
	}
}


void insertvar(int kind, gramtree *gt)
{
	if (!FindVarS(gt->id_type_name) && !FindArrayS(gt->id_type_name)) // 当前作用域没找到该变量
	{
		if (!FindStruct(gt->id_type_name)) // 没找到同名结构体
		{
			vartable *tempvar = (vartable *)malloc(sizeof(vartable));
			tempvar->name = (char *)malloc(sizeof(char) * (strlen(gt->id_type_name) + 1));
			strcpy(tempvar->name, gt->id_type_name);
			tempvar->type = kind;
			tempvar->snext = VARS[SCOPE];
			VARS[SCOPE] = tempvar;
			tempvar->next = VARTAB[hash_pjw(tempvar->name)]->next;
			tempvar->pre = VARTAB[hash_pjw(tempvar->name)];
			if (tempvar->next)
			{
				tempvar->next->pre = tempvar;
			}
		}
		else // 重名结构体
		{
			printf("Error type 3 at Line %d : 变量 %s 与结构体重名！", gt->line, gt->id_type_name);
		}
	}
	else // 变量名重复
	{
		printf("Error type 3 at Line %d : 变量 %s 出现重复定义！\n", gt->line, gt->id_type_name);
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
