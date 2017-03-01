#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#define MAX 10

double nqueens(int n);
int place(int k, int *X);

/**
* @brief	判断当前位置能否放置皇后
* @arg		k:	int型，表示当前行
* @arg		X:	int *型，存储当前放置方式
* @retval	int
* @return	返回能否放置，1能，0否
*/
int place(int k, int *X)
{
	int i = 0;
	while (i < k)
	{
		if ((X[i] == X[k]) || (abs(X[i]-X[k]) == abs(i-k)))
		{
			return 0;
		}
		i += 1;
	}
	return 1;
}

/**
* @brief	求解n-皇后问题的解的个数
* @arg		n:	int型，表示几皇后
* @retval	double型
* @return	解的个数
*/
double nqueens(int n)
{
	double answer_num = 0;	// 存储解的个数	
	int k, *X;			// k是当前行，X[k]是当前列
	X = (int *)malloc(sizeof(int) * n);

	X[0] = -1;
	k = 0;
	while (k >= 0)
	{
		X[k] = X[k] + 1;	// 移到下一行
		while (X[k] < n && !place(k, X)) // 判断此处能否放这个皇后
		{
			X[k] = X[k] + 1;
		}
		if (X[k] < n)	// 找到一个位置
		{
			if ((n-1) == k)	// 是一个完整解
			{
				answer_num++;	// 解的个数加1
			}
			else
			{// 转向下一行
				k += 1;
				X[k] = -1;
			}
		}
		else
		{
			k -= 1;	// 回溯
		}
	}
	return answer_num;
}

int main(int argc, char *argv[])
{
	int i;
	for (i = 1; i < MAX+1; i++)
	{
		printf("%d\t皇后  共%.0lf解\n", i, nqueens(i));
	}
	return 0;
}
