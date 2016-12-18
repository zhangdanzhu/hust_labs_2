#ifndef GRAMTREE_H
#define GRAMTREE_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// 定义每个符号表数组的最大容量
#define MAX 128 
// 定义作用域的最大嵌套数
#define SMAX 10

// 语法树结点
typedef struct gramtree {
	int unit_type;              // 类型（终结符或非终结符，0为终结符，-1为产生空的语法单元，主要用于语法树输出时判断是否要额外信息）
	int line;                   // 行号
	int type;					// 类型（0:int, 1:float, 2:array, 3:func, 4:struct)
	int type2;				// 返回值类型或数组元素类型
	char *struct_name;			// 结构体名
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

// 数组每个维度结点或函数的参数列表
typedef struct dim_arg {
	int num;
	int type;
	char *name;
	dim_arg *next;
}

/*// 结构体内容
typedef struct str {
	int type;
	char *name;
	str *next;
}*/

// 符号表结点
typedef struct symtable {
	char *name;					// 符号名
	int	type;					// 变量类型 0:int,1:float,2:array,3:func, 4:struct
	int type2;			     	// 返回值类型或数组元素类型
	char *struct_name;			// 若变量自身或返回值等为结构体类型，则记录下结构体名
	int dim_arg_num;			// 维度或参数数目
	dim_arg *list;				// 每一维长度或参数列表
	symtable *slist;					// 结构体内容
	// 联合体用于存放变量的值
	union {
		int int_val;
		float float_val;
	}
	vartable *next;				// 指向下一个符号表结点
	vartable *pre;				// 指向上一个符号表结点，方便删除当前层时调整openhash的链	
	vartable *snext;			// 指向当前层的下一个符号表结点
}symtable;	

symtable *SYMTAB[MAX];  // 符号表
vartable *SYMS[SMAX];   // 作用域链

int SCOPE;

gramtree * Create_Node(int lineno, char *lex_name, int num, ...);

void PreOrder_Traverse(struct gramtree* root, int level);

void CreateSymTab();

symtable *FindSym(char *name);

int FindSymS(char *name);

unsigned int hash_pjw(char *name);

#endif
