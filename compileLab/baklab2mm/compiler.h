#ifndef GRAMTREE_H
#define GRAMTREE_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// 定义每个符号表数组的最大容量
#define MAX 50 

// 语法树结点
typedef struct gramtree {
	int unit_type;              // 类型（0为终结符）
	int line;                   // 行号
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

// 变量表结点
typedef struct vartable {
	char *name;     			// 变量名
	int type;					// 变量类型
	// 联合体用于存放变量的值
	union {
		int int_val;
		float float_val;
	}
	vartable *next;				// 指向下一个变量表结点
	// TODO 多层作用域
}vartable;	

// 函数表结点
typedef struct functable {
	char *name;					// 函数名
	int return_type;			// 返回值类型
	int arg_num;				// 参数数目
	functable *next;			// 指向下一个函数表结点
	// TODO 多层作用域
}functable;

// 数组表结点
typedef struct arraytable {
	char *name;					// 数组名
	int diomend;				// 维度
	arraytable *next;			// 指向下一个数组表
	// TODO 多层作用域
}arraytable;

// 结构体结点
typedef struct structtable {
	char *name;					// 结构体名

}

vartale * VARTAB[]


gramtree * Create_Node(int lineno, char *lex_name, int num, ...);

void PreOrder_Traverse(struct gramtree* root, int level);

#endif
