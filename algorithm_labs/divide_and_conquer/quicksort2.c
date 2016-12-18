#include "divide.h"


/**
* @brief	quicksort迭代模型
* @arg		p:		int型，表示从下标为p的元素开始
* @arg		q:		int型，表示到下标为q的元素结束
* @arg		A:		int *型，指向待划分集合
* @arg		n:		int型，数组元素个数
* @retval	void
*/
void quicksort2(int p, int q, int *A, int n)
{
	int j;
	int top = -1;
	int *STACK = (int *)malloc(sizeof(int) * (int)floor(log((double)n) / log((double)2))); // 用数组实现栈

	while (1)
	{
		while (p < q)
		{
			j = q + 1;
			partition(p, &j, A);
			if ((j - p) < (q - j)) // 先对较小的子文件进行分类
			{
				STACK[top+1] = j + 1;
				STACK[top+2] = q;
				q = j - 1;
			}
			else
			{
				STACK[top+1] = p;
				STACK[top+2] = j - 1;
				p = j + 1;
			}
			top = top + 2;
		}
		if (top == -1)
		{
			return ;
		}
		// 再对较大的进行分类
		q = STACK[top];
		p = STACK[top-1];
		top = top - 2;
	}
}
