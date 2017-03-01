#include "divide.h"

/**
* @brief	用A[m]划分集合A[m:p-1]
* @arg		m:		int型，选取划分元素A[m]
* @arg		p:		int *型，选取A[p-1],其带着划分元素所在下标退出
* @arg		A:		int *型，指向待划分集合
* @retval	void
*/
void partition(int m, int *p, int *A)
{
	int i = m;		// i用于记录下标,初始为m
	int v = A[m];	// v用于暂存划分元素

	while (1)
	{
		// i由左向右移，直到找到大于划分元素的
		do {
			i += 1;
		}while (i != *p && A[i] < v);
		// p由右向左移，直到找到小于划分元素的
		do {
			*p -= 1;
		}while (A[*p] > v);

		if (i < *p)
		{
			interchange(&A[i],&A[*p]);
		}
		else
		{
			break;
		}
	}
	A[m] = A[*p];
	A[*p] = v;
}

