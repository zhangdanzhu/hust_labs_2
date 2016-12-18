#ifndef GRAMTREE_H
#define GRAMTREE_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// 定义符号表数组的最大容量
#define MAX 128 
// 定义作用域的最大嵌套数
#define SMAX 10

// 符号表结点
typedef struct symtable {
	char *name;					// 符号名
	int	type0;					// 变量类型 0:int,1:float,2:array,3:func, 4:struct
	int size;					// 维度或参数数目
	int kind;					// 表示是否为结构体类型声明，1是
	struct symtable *type;				// 具体类型或返回类型
	struct symtable *args;				// 参数列表
	// 联合体用于存放变量的值
	union {
		int int_val;
		float float_val;
	};
	struct symtable *next;				// 指向下一个符号表结点
	struct symtable *pre;				// 指向上一个符号表结点，方便删除当前层时调整openhash的链	
	struct symtable *snext;			// 指向当前层的下一个符号表结点
}symtable;

// 语法树结点
typedef struct gramtree {
	int unit_type;              // 类型（终结符或非终结符，0为终结符，-1为产生空的语法单元，主要用于语法树输出时判断是否要额外信息）
	int line;                   // 行号
	int type0;					// 类型（0:int, 1:float, 2:array, 3:func, 4:struct)
	symtable *type;				// 具体类型
	int leftval;				// 标记是否为左值
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

symtable *SYMTAB[MAX];  // 符号表
symtable *SYMS[SMAX];   // 作用域链

int SCOPE;   // 作用域
int NOERROR; // 用于判断是否需要打印语法树

gramtree * Create_Node(int lineno, char *lex_name, int num, ...);

void PreOrder_Traverse(struct gramtree* root, int level);

// 初始化符号表
void CreateSymTab();
// 判断当前变量是否在符号表中
symtable *FindSym(char *name, int scale, int strkind);
// 获取当前作用域符号链（用于struct和函数参数）
symtable *GetScope();
// 删除当前作用域
void DeleteScope();
// 处理ExtDef
void DealExtDef(int kind, gramtree *gt1, gramtree *gt2, gramtree *gt3);
// 处理CompSt
void DealCompSt(gramtree *compst, symtable *rettype);
// 处理Stmt
void DealStmt(gramtree *stmt, symtable *rettype);
// 处理Exp,确定类型
void DealExp(gramtree *exp);
// 查找域
symtable *FindField(symtable *structsym, char *fieldname);
// 处理RETURN
void DealReturn(gramtree *retnode, symtable *rettype);
// 处理IF
void DealIf(gramtree *ifnode, symtable *rettype);
// 处理WHILE
void DealWhile(gramtree *whilenode, symtable *rettype);
// 处理VarList
void DealVarList(gramtree *vl);
// 处理ExtDecList
void DealExtdl(gramtree *gt2, symtable *temptype);
// 处理specifier
symtable *DealSpecifier(gramtree *spe);
// 处理DefList 
symtable *DealDefList(gramtree *dl, int stru);
// 处理DecList
void DealDecList(gramtree *dl, symtable *sptype, int stru);
// 处理VarDec
void DealVarDec(gramtree *vd, symtable *sptype);
// 加入符号表
void InsertSym(symtable *newsym);
// 比较两个类型是否相等
int CompType(symtable *type1, symtable *type2);
// 创建新符号表结点
symtable *CreateSymNode();


unsigned int hash_pjw(char *name);

#endif
