#include "divide.h"

void main()
{
	int i;
	int f;
	int *A;		// 指向待排序集合
	int n;		// 元素个数
	do {
		f = 0;
		printf("请输入集合元素个数：");
		scanf("%d", &n);
		A = (int *)malloc(sizeof(int) * n);
		printf("请依次输入集合中元素：");
		for (i = 0; i < n; i++)
		{
			scanf("%d", &A[i]);
		}
		quicksort2(0, n-1, A, n);
		printf("排序后的集合为： ");
		for (i = 0; i < n; i++)
		{
			printf("%d ", A[i]);
		}
		printf("\n");
		printf("是否继续（1是 0否）：");
		scanf("%d", &f);
	}while (f);
}
