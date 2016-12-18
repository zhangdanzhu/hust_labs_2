#include "gramtree.h"
#include "stdarg.h"

gramtree * Create_Node(int lineno, char *lex_name, int num, ...)
{
	int i = 0;
	va_list valist;
	va_start(valist, num);
	gramtree *lchild;
	
	gramtree *new_node = (gramtree *)malloc(sizeof(gramtree));
	new_node->unit_type = num;
	new_node->line = lineno;
	new_node->lex_unit_name = lex_name;
	new_node->l_child = NULL;
	new_node->r_child = NULL;

	if (num > 0)
	{	
		lchild = va_arg(valist, gramtree *);
		new_node->l_child = lchild;
		new_node->line = lchild->line;

		if (num >= 2)
		{
			for (i = 0; i < num-1; i++)
			{
				lchild->r_child = va_arg(valist, gramtree *);
				lchild = lchild->r_child;
			}
		}
	}
	return new_node;
}

void PreOrder_Traverse(struct gramtree* root, int level)
{
	int i = 0;
	if (root == NULL)
	{
		return;
	}
	if (root->unit_type >= 0) // 产生空的不显示
	{
		for ( ; i < level; i++)
		{
			printf("  ");  // 缩进以显示父子关系
		}
		printf("%s", root->lex_unit_name);
		// 若是终结符
		if (!root->unit_type)
		{
			if (!strcmp(root->lex_unit_name, "ID") || !(strcmp(root->lex_unit_name, "TYPE")))
			{
				printf(": %s\n", root->id_type_name);
			}
			else if (!strcmp(root->lex_unit_name, "INT"))
			{
				printf(": %d\n", root->int_val);
			}
			else if (!strcmp(root->lex_unit_name, "FLOAT"))
			{
				printf(": %f\n", root->float_val);
			}
			else
			{
				printf("\n");
			}
		}
		// 否则非终结符需打印行号
		else
		{
			printf(" (%d) \n", root->line);
		}
	}
	PreOrder_Traverse(root->l_child, level+1); // 先序遍历左孩子结点，层数+1
	PreOrder_Traverse(root->r_child, level);   // 先序遍历右孩子结点（即兄弟），层数不变
}

