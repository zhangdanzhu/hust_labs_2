#include "stackh.h"

void initPOSTK(POSTK *const p, int m)		//初始化p指的栈：最多m个元素
{
	p->elems = new int[m];
	p->max = m;
	p->pos = 0;
}
void initPOSTK(POSTK *const p, const POSTK&s) //用栈s初始化p指的栈
{
	p->elems = new int[s.max];

}
int size(const POSTK *const p)				//返回p指栈的最大元素个数max
{
	return p->max;
}

int  howMany (const POSTK *const p)		//返回p指栈的实际元素个数pos
{
	return p->pos;
}

int  getelem (const POSTK *const p, int x)	//取下标x处的栈元素
{
	return p->elems[x];
}

POSTK *const push(POSTK *const p, int e) 	//将e入栈，并返回p值
{
	if (p->pos < p->max) {
		p->elems[p->pos++] = e;
	}
	return p;
}

POSTK *const pop(POSTK *const p, int &e) 	//出栈到e，并返回p值
{
	if (p->pos){
		e = p->elems[(p->pos--)-1];
	}
	return p;
}

POSTK *const assign(POSTK *const p, const POSTK&s) //赋s给p指的栈，返p值
{

	return p;
}

void print(const POSTK*const p)				//打印p指向的栈
{
	int i = 0;
	for (i = p->pos-1; i >= 0; i--){
		printf("%d ", p->elems[i]);
	}
	printf("\n");
}

void destroyPOSTK(POSTK*const p)			//销毁p指向的栈
{
	if (p){
		delete p->elems;
		delete p;
	}
}
