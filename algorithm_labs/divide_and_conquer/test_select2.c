#include "divide.h"

void main()
{
	int i;
	int f;
	int *A;		// 指向待排序集合
	int n;		// 元素个数
	int k;		// 要找第k小元素
	int r;		// 每组元素个数
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
		printf("请输入要找第几小元素（1 到 %d）：", n);
		scanf("%d", &k);
		printf("请输入每组元素个数(大于1的整数)：");
		scanf("%d", &r);
		printf("第%d小元素为：%d", k, A[select2(A, 0, n-1, k, r)]);
		printf("\n");
		printf("是否继续（1是 0否）：");
		scanf("%d", &f);
	}while (f);
}
