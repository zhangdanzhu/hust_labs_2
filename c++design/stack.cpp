#include "stackh.h"

STACK::STACK(int m):elems(new int[m]), max(elems?m:0)  //��ʼ��ջ�����m��Ԫ��
{
	pos = 0;
}


STACK::STACK(const STACK&s):elems(new int[s.size()]), max(elems?s.size():0) 	//��ջs������ʼ��ջ
{
	if (elems){
        for (int i = 0; i < max; i++){
			elems[i] = s[i];
		}
		pos = s;
	}
	else {
		pos = 0;
		cout<<"��ʼ��ʧ�ܣ�"<<endl;
    }
}

int STACK::size() const				//����ջ�����Ԫ�ظ���max
{
	return max;
}

STACK::operator int ( ) const			//����ջ��ʵ��Ԫ�ظ���pos
{
	return pos;
}

int STACK::operator[ ](int x) const		//ȡ�±�x����ջԪ��
{
	if (x >= 0 && x < pos){
		return elems[x];
	}
	cout<<"���±�Ƿ���"<<endl;
	return (-1);
}

STACK& STACK::operator<<(int e) 	//��e��ջ�������ص�ǰջ
{
	if (pos < max){
		elems[pos++] = e;
	}
	else {
		cout<<"ջ������"<<endl;
	}
	return *this;
}

STACK& STACK::operator>>(int &e)	//��ջ��e�������ص�ǰջ
{
	if (pos != 0){
		e = elems[pos--];
	}
	else {
		cout<<"ջΪ�գ�"<<endl;
	}
	return *this;
}

STACK& STACK::operator=(const STACK&s) //��s����ǰջ�����ظ�ջ
{
	// TODO
	return *this;
}

void STACK::print() const				//��ӡջ
{
	if (pos){
		for (int i = pos-1; i >= 0; i--){
			cout<<elems[i];
		}
		cout<<endl;
	}
	else {
		cout<<"ջΪ�գ�"<<endl;
	}
}

STACK::~STACK( )						//����ջ
{
	if (elems){
		delete elems;
	}
}
