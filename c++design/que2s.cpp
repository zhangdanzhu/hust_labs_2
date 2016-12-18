#include "stackh.h"

QUE2S::QUE2S(int m):s1(m),s2(m){}			//初始化队列：每栈最多m个元素

QUE2S::QUE2S(const QUE2S &q):s1(q.s1), s2(q.s2){}  //用队列q拷贝构造新队列

QUE2S::operator int ( ) const					//返回队列的实际元素个数
{
	return s1+s2;
}

QUE2S& QUE2S::operator<<(int e) 						//将e入队列，并返回当前队列
{
	if (s1 == s1.size()){
		cout<<"已满！禁止入队！"<<endl;
	}
	s1<<e;
	return *this;
}
QUE2S& QUE2S::operator>>(int &e)			//出队列到e，并返回当前队列
{
	if (0 == s2){
		while(int(s1)) {
			s1>>e;
			s2<<e;
		}   // 要改！！！！！！！！！！！！！！！！
	}
	s2>>e;
}

QUE2S& QUE2S::operator=(const QUE2S &q)//赋q给当前队列并返回该队列
{
	// TODO
	return *this;
}

void QUE2S::print( ) const						//打印队列
{
	if (s1 != 0 && s2 != 0){
		cout<<"队列为空！"<<endl;
	}
	for (int i = s2 - 1; i >= 0; i--){
		cout<<s2[i]<<endl;
	}
	for
}

QUE2S::~QUE2S( )							//销毁队列
{

}

