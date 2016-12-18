#ifndef GRAMTREE_H
#define GRAMTREE_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// 定义每个符号表数组的最大容量
#define MAX 50 

// 语法树结点
typedef struct gramtree {
	int unit_type;              // 类型（终结符或非终结符，0为终结符，-1为产生空的语法单元，主要用于语法树输出时判断是否要额外信息）
	int line;                   // 行号
	int *type;					// 类型（0:int, 1:float, 2:array, 3:func)
	char* lex_unit_name;        // 词法单元名
	struct gramtree* l_child;   // 左孩子结点
	struct gramtree* r_child;   // 右孩子结点
	// 联合体用于存放额外打印信息
	union { 
		char *id_type_name;     // 标识符对应词素或具体类型
		int int_val;			// int型的值
		float float_val;		// float型的值
	};
}gramtree;

// 数组每个维度结点
typedef struct dim {
	int num;
	dim *next;
}

// 变量表结点
typedef struct vartable {
	char *name;     			// 变量名
	int type;					// 变量类型
	char *struct_name;			// 若类型为结构体，则此为结构体名
	// 联合体用于存放变量的值
	union {
		int int_val;
		float float_val;
	}
	vartable *next;				// 指向下一个变量表结点
	vartable *pre;				// 指向上一个变量表结点，方便删除当前层时调整openhash的链	
	vartable *snext;			// 指向当前层的下一个变量表结点
}vartable;	

// 函数表结点
typedef struct functable {
	char *name;					// 函数名
	int return_type;			// 返回值类型
	int arg_num;				// 参数数目
	functable *next;			// 指向下一个函数表结点
}functable;

// 数组表结点
typedef struct arraytable {
	char *name;					// 数组名
	int type;					// 数组类型
	int dimension;				// 维度
	dim *list;					// 每一维的数量
	arraytable *next;			// 指向下一个数组表
	arraytable *pre;			// 指向上一个数组表结点，方便删除当前层时调整openhash的链
	arraytable *snext;			// 指向当前层的下一个数组表结点
}arraytable;

// 结构体结点
typedef struct structtable {
	char *name;					// 结构体名
	structtable *next;			// 指向下一个结构体表结点
}structtable;

vartable *VARTAB[128];
functable *FUNCTAB[128];
arraytable *ARRAYTAB[128];
structtable *STRUCTTAB[128];
vartable *VARS[10];
arraytale *ARRAYS[10];


int SCOPE;

gramtree * Create_Node(int lineno, char *lex_name, int num, ...);

void PreOrder_Traverse(struct gramtree* root, int level);

void CreateSymTab();

int FindVar(char *name);

unsigned int hash_pjw(char *name);

#endif
