#include "stdio.h"
#include "iostream"

using namespace std;

//=========================������̵�STACK========================//
struct POSTK{
	int  *elems;				//�����ڴ����ڴ��ջ��Ԫ��
	int   max;					//ջ�ܴ�ŵ����Ԫ�ظ���
	int   pos;					//ջʵ������Ԫ�ظ�����ջ��ʱpos=0;
};
void initPOSTK(POSTK *const p, int m);		//��ʼ��pָ��ջ�����m��Ԫ��
void initPOSTK(POSTK *const p, const POSTK&s); //��ջs��ʼ��pָ��ջ
int  size (const POSTK *const p);				//����pָջ�����Ԫ�ظ���max
int  howMany (const POSTK *const p);		//����pָջ��ʵ��Ԫ�ظ���pos
int  getelem (const POSTK *const p, int x);	//ȡ�±�x����ջԪ��
POSTK *const push(POSTK *const p, int e); 	//��e��ջ��������pֵ
POSTK *const pop(POSTK *const p, int &e); 	//��ջ��e��������pֵ
POSTK *const assign(POSTK*const p, const POSTK&s); //��s��pָ��ջ����pֵ
void print(const POSTK*const p);				//��ӡpָ���ջ
void destroyPOSTK(POSTK*const p);			//����pָ���ջ

//========================��������STACK=========================//
class OOSTK{
    int  *const  elems;		//�����ڴ����ڴ��ջ��Ԫ��
    const  int   max;			//ջ�ܴ�ŵ����Ԫ�ظ���
    int   pos;					//ջʵ������Ԫ�ظ�����ջ��ʱpos=0;
public:
    OOSTK(int m);			//��ʼ��ջ�����m��Ԫ��
    OOSTK(const OOSTK&s);//��ջs������ʼ��ջ
    int  size ( ) const;			//����ջ�����Ԫ�ظ���max
    int  howMany ( ) const;	//����ջ��ʵ��Ԫ�ظ���pos
    int  getelem (int x) const;	//ȡ�±�x����ջԪ��
    OOSTK& push(int e); 		//��e��ջ�������ص�ǰջ
    OOSTK& pop(int &e); 	//��ջ��e�������ص�ǰջ
    OOSTK& assign(const OOSTK&s); 	//��s��ջ�������ر���ֵ�ĵ�ǰջ
    void print( ) const;						//��ӡջ
    ~OOSTK( );							//����ջ
};

//===================�����������������STACK=====================//
class STACK{
    int  *const  elems;		//�����ڴ����ڴ��ջ��Ԫ��
    const  int   max;			//ջ�ܴ�ŵ����Ԫ�ظ���
    int   pos;					//ջʵ������Ԫ�ظ�����ջ��ʱpos=0;
public:
    STACK(int m);				//��ʼ��ջ�����m��Ԫ��
    STACK(const STACK&s); 			//��ջs������ʼ��ջ
    virtual int  size ( ) const;				//����ջ�����Ԫ�ظ���max
    virtual operator int ( ) const;			//����ջ��ʵ��Ԫ�ظ���pos
    virtual int operator[ ] (int x) const;		//ȡ�±�x����ջԪ��
    virtual STACK& operator<<(int e); 	//��e��ջ�������ص�ǰջ
    virtual STACK& operator>>(int &e);	//��ջ��e�������ص�ǰջ
    virtual STACK& operator=(const STACK&s); //��s����ǰջ�����ظ�ջ
    virtual void print( ) const;				//��ӡջ
	virtual ~STACK( );						//����ջ
};

//====================��2��STACK��ɵĶ���========================//
class QUE2S{
    STACK s1, s2;							//һ�����п�������ջ�ۺ϶���
public:
    QUE2S(int m);						//��ʼ�����У�ÿջ���m��Ԫ��
    QUE2S(const QUE2S &q);			//�ö���q���������¶���
    operator int ( ) const;					//���ض��е�ʵ��Ԫ�ظ���
    QUE2S& operator<<(int e); 			//��e����У������ص�ǰ����
    QUE2S& operator>>(int &e);			//�����е�e�������ص�ǰ����
    QUE2S& operator=(const QUE2S &q);//��q����ǰ���в����ظö���
    void print( ) const;						//��ӡ����
    ~QUE2S( );							//���ٶ���
};

//======================��STACK�̳еĶ���=========================//
class QUEIS:  public  STACK{		//STACK��Ϊ���ɶ��еĵ�1��ջ
    STACK  s;						//s��Ϊ���ɶ��еĵ�2��ջ
public:
    QUEIS(int m);						//��ʼ�����У�ÿջ���m��Ԫ��
    QUEIS(const QUEIS &q); 		//�ö���q������ʼ������
    virtual operator int ( ) const;		//���ض��е�ʵ��Ԫ�ظ���
    virtual QUEIS& operator<<(int e); 	//��e����У������ص�ǰ����
    virtual QUEIS& operator>>(int &e);//�����е�e�������ص�ǰ����
    virtual QUEIS& operator=(const QUEIS &q); //��q�����в����ظö���
    virtual void print( ) const;			//��ӡ����
    virtual ~QUEIS( );					//���ٶ���
};

