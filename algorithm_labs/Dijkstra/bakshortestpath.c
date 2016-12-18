#include <stdio.h>
#include <stdlib.h>

#define MIN(a,b) (a<b)?a:b
#define	MAXD 11111

typedef struct node {
	int v;	// 结点号
	struct node *next;
}node;

typedef struct nodelist {
	struct node *head; // 指向链表头
	struct node *tail; // 指向链尾
}nodelist;

/**
* @brief	获取并old的路径并在此基础上增加
* @arg		new:	nodelist *型，指向待修改nodelist
* @arg		old:	nodelist *型，指向添加进的nodelist
* @arg		v:		int型，指向添加的结点号
* @retval	void
*/
void get_add_nl(nodelist *new, nodelist *old, int v)
{
	node *tempnode = (node *)malloc(sizeof(node));
	node *temp = old->head;
	tempnode->next = NULL;
	if (temp != NULL)
	{
		tempnode->v = temp->v;
		new->head = tempnode;
		new->tail = tempnode;
		temp = temp->next;
		while (temp != NULL)
		{
			tempnode = (node *)malloc(sizeof(node));
			tempnode->v = temp->v;
			tempnode->next = new->tail->next;
			new->tail->next = tempnode;
			new->tail = new->tail->next;
			temp = temp->next;
		}
		tempnode = (node *)malloc(sizeof(node));
		tempnode->v = v;
		tempnode->next = new->tail->next;
		new->tail->next = tempnode;
		new->tail = new->tail->next;
	}
	else
	{
		tempnode->v = v;
		new->head = tempnode;
		new->tail = tempnode;
	}
}

/**
* @brief	Dijkstra算法生成最短路径
* @arg		v:		int型，表示源点标号
* @arg		COST:	int *型，指向成本邻接矩阵
* @arg		DIST:	int *型，存放结点v到结点j的最短路径长度
* @arg		n:		结点个数
* @retval
*/
void shortest_paths(int v, int *COST, int *DIST, int n, nodelist *NODELIST)
{
	int *S = (int *)malloc(sizeof(int) * n);
	int u, num, i, w;
	node *tempnode = (node *)malloc(sizeof(node));
	for (i = 0; i < n; i++)
	{
		S[i] = 0;
		DIST[i] = COST[v*n+i];	// 即COST[v][i] 
	}
	// 结点v计入S
	S[v] = 1;
	DIST[v] = 0;
	// 初始化各路径
	NODELIST[v].head = NULL;
	NODELIST[v].tail = NULL;
	for (w = 0; w < n; w++)
	{
		if (S[w] == 0)
		{
			get_add_nl(&NODELIST[w], &NODELIST[v], v + 1);
		}
	}

	for (num = 1; num < n; num++)	// 确定由结点v出发的n-1条路
	{
		u = -1;
		for (w = 0; w < n; w++)		// 找出最小的DIST[u]
		{
			if (0 == S[w])
			{
				if (-1 == u)
				{
					u = w;
				}
				else
				{
					if (DIST[w] < DIST[u])
					{
						u = w;
					}
				}
			}
		}
		S[u] = 1;	// 结点u计入S
		for (w = 0; w < n; w++)
		{
			if (0 == S[w])
			{
				// DIST[w] = MIN(DIST[w], DIST[u] + COST[u*n+w]);
				if ((DIST[u]+COST[u*n+w]) < DIST[w])
				{
					DIST[w] = DIST[u] + COST[u*n+w];
					get_add_nl(&NODELIST[w], &NODELIST[u], u + 1);
				}
			}
		}
	}
}	


int main(int argc, int *argv[])
{
	int n;		// 结点数
	int v;		// 源点标号
	int *DIST;	// 存放最短路径长度
	int *COST;	// 指向成本邻接矩阵
	nodelist *NODELIST; // 存放路径
	int i, j;

	/*printf("请输入结点个数：");
	scanf("%d", &n);
	COST = (int *)malloc(sizeof(int) * n * n);
	DIST = (int *)malloc(sizeof(int) * n);
	NODELIST = (nodelist *)malloc(sizeof(nodelist) * n);
	printf("请选择源点：(1到%d)", n);
	scanf("%d", &v);
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
	DIST = (int *)malloc(sizeof(int) * n);
	NODELIST = (nodelist *)malloc(sizeof(nodelist) * n);
	v = 1;
	int a[49] = {0, 20, 50, 30, MAXD, MAXD, MAXD,
			MAXD, 0, 25, MAXD, MAXD, 70, MAXD,
			MAXD, MAXD, 0, 40, 25, 50, MAXD,
			MAXD, MAXD, MAXD, 0, 55, MAXD, MAXD,
			MAXD, MAXD, MAXD, MAXD, 0, 10, 70,
			MAXD, MAXD, MAXD, MAXD, MAXD, 0, 50,
			MAXD, MAXD, MAXD, MAXD, MAXD, MAXD, 0};
	COST = a;
	
	shortest_paths(v-1, COST, DIST, n, NODELIST);
	printf("start\tend\tlength\tnodes list\n");
	for (i = 0; i < n; i++)
	{
		if ((i+1) != v)
		{
			printf("V%d\tV%d\t%d\t", v, i+1, DIST[i]);
			node *temp = NODELIST[i].head;
			while (temp != NULL)
			{
				printf("V%d ", temp->v);
				temp = temp->next;
			}
			printf("V%d", i+1);
			printf("\n");
		}
	}
	return 0;
}
