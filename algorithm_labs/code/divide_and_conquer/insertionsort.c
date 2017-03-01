#include "divide.h"

/**
* @brief	插入排序(非降次序)
* @arg		A:	int *型，指向待排序集合
* @arg		i:	int型，起始下标
* @arg		j:	int型，截止下标
* @retval	void
*/
void insertionsort(int *A, int i, int j)
{
	int k;
	int l = i;
	int item; 
	for (k = i + 1; k <= j; k++)
	{
		item = A[k];
		i = k - 1;
		while ((i != (l-1)) && item < A[i])
		{
			A[i+1] = A[i]; 
			i = i - 1;
		}
		A[i+1] = item;
	}
}
