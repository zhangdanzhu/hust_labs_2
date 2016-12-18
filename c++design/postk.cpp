#include "stackh.h"

void initPOSTK(POSTK *const p, int m)		//��ʼ��pָ��ջ�����m��Ԫ��
{
	p->elems = new int[m];
	p->max = m;
	p->pos = 0;
}
void initPOSTK(POSTK *const p, const POSTK&s) //��ջs��ʼ��pָ��ջ
{
	p->elems = new int[s.max];

}
int size(const POSTK *const p)				//����pָջ�����Ԫ�ظ���max
{
	return p->max;
}

int  howMany (const POSTK *const p)		//����pָջ��ʵ��Ԫ�ظ���pos
{
	return p->pos;
}

int  getelem (const POSTK *const p, int x)	//ȡ�±�x����ջԪ��
{
	return p->elems[x];
}

POSTK *const push(POSTK *const p, int e) 	//��e��ջ��������pֵ
{
	if (p->pos < p->max) {
		p->elems[p->pos++] = e;
	}
	return p;
}

POSTK *const pop(POSTK *const p, int &e) 	//��ջ��e��������pֵ
{
	if (p->pos){
		e = p->elems[(p->pos--)-1];
	}
	return p;
}

POSTK *const assign(POSTK *const p, const POSTK&s) //��s��pָ��ջ����pֵ
{

	return p;
}

void print(const POSTK*const p)				//��ӡpָ���ջ
{
	int i = 0;
	for (i = p->pos-1; i >= 0; i--){
		printf("%d ", p->elems[i]);
	}
	printf("\n");
}

void destroyPOSTK(POSTK*const p)			//����pָ���ջ
{
	if (p){
		delete p->elems;
		delete p;
	}
}