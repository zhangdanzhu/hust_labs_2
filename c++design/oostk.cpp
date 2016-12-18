#include "stackh.h"

OOSTK::OOSTK(int m): elems(new int[m]), max(elems?m:0)       // ��ʼ��ջ�����m��Ԫ��
{
	pos = 0;
}

OOSTK::OOSTK(const OOSTK&s):elems(new int[s.size()]), max(elems?s.size():0)   // ��ջs������ʼ��ջ
{
	if (elems){
    	for (int i = 0; i < max; i++){
			elems[i] = s.getelem(i);
		}
		pos = s.howMany();
	}
	else {
		pos = 0;
		cout<<"��ʼ��ʧ�ܣ�"<<endl;
    }
}

int OOSTK::size() const         // ����ջ�����Ԫ�ظ���max
{
	return max;
}

int OOSTK::howMany() const		   //����ջ��ʵ��Ԫ�ظ���pos
{
	return pos;
}

int OOSTK::getelem(int x) const	   //ȡ�±�x����ջԪ��
{
	return elems[x];
}

OOSTK& OOSTK::push(int e) 		//��e��ջ�������ص�ǰջ
{
	if (max > pos){
		elems[pos++] = e;
	}
	else {
		cout<<"ջ��������ջʧ�ܣ�\n";
	}
	return *this;
}

OOSTK& OOSTK::pop(int &e) 	    //��ջ��e�������ص�ǰջ
{
	if (0 == pos){
		cout<<"ջ�ѿգ���ջʧ�ܣ�\n";
	}
	else {
		e = elems[pos--];
	}
	return *this;
}

OOSTK& OOSTK::assign(const OOSTK&s) 	//��s��ջ�������ر���ֵ�ĵ�ǰջ
{
	// TODO:
	return *this;
}

void OOSTK::print() const						//��ӡջ
{
	if (0 == pos){
		printf("ջΪ�գ�\n");
	}
	else {
		for (int i = pos-1; i >= 0; i--){
			cout<<elems[i]<<" ";
		}
	}
	cout<<"\n";
}

OOSTK::~OOSTK()							//����ջ
{
	free(elems);
}