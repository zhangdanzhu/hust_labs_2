#include "divide.h"

/**
* @brief	基于二次取中的选择算法
* @arg		A:		int *型，指向元素集合
* @arg		m:		int型，起始下标
* @arg		p:		int型，截止下标
* @arg		k:		int型，要查第k小元素
* @arg		r:		int型，每组元素个数
* @retval	int
* @return	第k小元素下标 
*/
int select2(int *A, int m, int p, int k, int r)
{
	int n, i, j;
	if ((p-m+1) <= r)	// 只有一组则直接用insertionsort
	{
		insertionsort(A, m, p);
		return (m + k - 1);
	}
	while (1)
	{
		n = p - m + 1;		//元素数

		for (i = 1; i < floor((double)n/r); i++)			
		{
			// 计算中间值
			insertionsort(A, m + (i-1) * r, m + i * r - 1);
			// 将中间值收集到A[m, p]的前部
			interchange(&A[m+i-1], &A[m + (i-1) * r + (int)floor((double)r/2) - 1]);
		}
		// 二次取中
		j = select2(A, m, m + floor((double)n/r) - 1, ceil(floor((double)n/r) / 2), r);
		interchange(&A[m], &A[j]);	// 产生划分元素
		j = p + 1;
		partition(m, &j, A);
		if ((j-m+1) == k)
		{
			return j;
		}
		else if ((j-m+1) > k)	// 偏大则在小的一堆中查
		{
			p = j - 1;
		}
		else
		{
			k = k - (j-m+1);
			m = j + 1;
		}
	}
}
