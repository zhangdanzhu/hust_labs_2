#include "stackh.h"

QUE2S::QUE2S(int m):s1(m),s2(m){}			//��ʼ�����У�ÿջ���m��Ԫ��

QUE2S::QUE2S(const QUE2S &q):s1(q.s1), s2(q.s2){}  //�ö���q���������¶���

QUE2S::operator int ( ) const					//���ض��е�ʵ��Ԫ�ظ���
{
	return s1+s2;
}

QUE2S& QUE2S::operator<<(int e) 						//��e����У������ص�ǰ����
{
	if (s1 == s1.size()){
		cout<<"��������ֹ��ӣ�"<<endl;
	}
	s1<<e;
	return *this;
}
QUE2S& QUE2S::operator>>(int &e)			//�����е�e�������ص�ǰ����
{
	if (0 == s2){
		while(int(s1)) {
			s1>>e;
			s2<<e;
		}   // Ҫ�ģ�������������������������������
	}
	s2>>e;
}

QUE2S& QUE2S::operator=(const QUE2S &q)//��q����ǰ���в����ظö���
{
	// TODO
	return *this;
}

void QUE2S::print( ) const						//��ӡ����
{
	if (s1 != 0 && s2 != 0){
		cout<<"����Ϊ�գ�"<<endl;
	}
	for (int i = s2 - 1; i >= 0; i--){
		cout<<s2[i]<<endl;
	}
	for
}

QUE2S::~QUE2S( )							//���ٶ���
{

}

