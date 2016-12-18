#ifndef DIVIDE_H
#define	DIVIDE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// 函数声明
void partition(int m, int *p, int *A);
void quicksort2(int p, int q, int *A, int n);
void interchange(int *x, int *y);
void insertionsort(int *A, int i, int j);
int select2(int *A, int m, int p, int k, int r);

#endif
