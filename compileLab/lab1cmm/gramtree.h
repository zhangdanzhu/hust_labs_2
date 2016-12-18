#ifndef GRAMTREE_H
#define GRAMTREE_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

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

gramtree * Create_Node(int lineno, char *lex_name, int num, ...);

void PreOrder_Traverse(struct gramtree* root, int level);

#endif
