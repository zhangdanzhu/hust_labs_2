#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/sem.h>

#define MYKEY 1111

// P、V操作的函数
void P(int semid, int index);
void V(int semid, int index);
// 信号量、累加中间结果、线程句柄
int semid;
int sum = 0;
pthread_t p1, p2;

// 线程执行函数
void *subp1();
void *subp2();


union semun {
	int   val;
	struct   semid_ds  *buf;
	unsigned short   *array;
};

// 线程1负责计算（1到100的累加，每次加一个数）
void *subp1()
{
	int i;
	for (i = 1; i <= 100; i++)
	{
		P(semid, 0);
		sum += i;
		V(semid, 1);
	}
	return NULL;
}

// 线程2负责打印（输出累加的中间结果）
void *subp2()
{
	int j;
	// 从0开始输出
	for (j = 1; j <= 100; j++)
	{
		P(semid, 1);
		printf("%d\n", sum);
		V(semid, 0);
	}
	return NULL;
}

void P(int semid, int index)
{
	struct sembuf sem;
	sem.sem_num = index;
	sem.sem_op = -1; // 申请一个共享资源
	sem.sem_flg = 0; // 操作标记：0或IPC_NOWAIT等
	semop(semid, &sem, 1); // 1:表示执行命令的个数
	return; 
}


void V(int semid, int index)
{
	struct sembuf sem;
	sem.sem_num = index;
	sem.sem_op = 1; // 释放一个共享资源
	sem.sem_flg = 0;
	semop(semid, &sem, 1);
	return;
}

int main(void)
{
	union semun  sem_args;
	unsigned short array[2]={1,0};
	sem_args.array = array;
	// 创建信号灯，两个信号灯用于两线程交替执行
	semid = semget(MYKEY,2,IPC_CREAT|0666);
	// 信号灯赋初值
	semctl(semid, 1, SETALL, sem_args); 
	// 创建两个线程subp1、subp2
	pthread_create(&p1, NULL, &subp1, NULL);
	pthread_create(&p2, NULL, &subp2, NULL);
	// 等待两个线程运行结束
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	// 删除信号灯
	semctl(semid, 2, IPC_RMID);
	return 0;
}
