#include <stdio.h>
#include <stdlib.h>

#define MIN(a,b) (a<b)?a:b
#define MAXD 11111

/**
* @brief	计算每对结点之间的最短路径长度
* @arg		COST:	int *型，成本邻接矩阵
* @arg		A:		int *型，A[i][i]是结点Vi到Vj的最短路径的成本
* @arg		n:		int型，结点个数
* @retval	void
*/
void all_paths(int *COST, int *A, int n)
{
	int i, j, k;
	for (i = 0; i < n; i++) // 将COST[i][j]复制到A[i][j]
	{
		for (j = 0; j < n; j++)
		{
			A[i*n+j] = COST[i*n+j];
		}
	}
	for (k = 0; k < n; k++) // 对最高下标为k的结点路径
	{
		// 对于所有可能的结点对
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				A[i*n+j] = MIN(A[i*n+j], A[i*n+k]+A[k*n+j]);
			}
		}
	}
}


int main(int argc, int *argv[])
{
	
	int n;		// 结点数
	int *A;		// 存放最短路径长度
	int *COST;	// 指向成本邻接矩阵
	// nodelist *NODELIST; // 存放路径
	int i, j, v;

	/*printf("请输入结点个数：");
	scanf("%d", &n);
	COST = (int *)malloc(sizeof(int) * n * n);
	A = (int *)malloc(sizeof(int) * n * n);
	NODELIST = (nodelist *)malloc(sizeof(nodelist) * n);
	printf("请输入邻接矩阵：\n");
	for(i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			scanf("%d", &COST[i*n+j]);
		}
	}*/

	n = 7;
	COST = (int *)malloc(sizeof(int) * n * n);
	A = (int *)malloc(sizeof(int) * n * n);
	// NODELIST = (nodelist *)malloc(sizeof(nodelist) * n);
	int a[49] = {0, 20, 50, 30, MAXD, MAXD, MAXD,
			MAXD, 0, 25, MAXD, MAXD, 70, MAXD,
			MAXD, MAXD, 0, 40, 25, 50, MAXD,
			MAXD, MAXD, MAXD, 0, 55, MAXD, MAXD,
			MAXD, MAXD, MAXD, MAXD, 0, 10, 70,
			MAXD, MAXD, MAXD, MAXD, MAXD, 0, 50,
			MAXD, MAXD, MAXD, MAXD, MAXD, MAXD, 0};
	COST = a;
	
	all_paths(COST, A, n);
	printf("start\tend\tlength\n");
	for (v = 0; v < n; v++)
	{
		for (i = 0; i < n; i++)
		{
			if (i != v)
			{
				printf("V%d\tV%d\t%d\t", v+1, i+1, A[v*n+i]);
				/*node *temp = NODELIST[i].head;
				while (temp != NULL)
				{
					printf("V%d ", temp->v);
					temp = temp->next;
				}*/
				printf("\n");
			}
		}
	}
	return 0;
}
