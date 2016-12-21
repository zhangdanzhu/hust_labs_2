#ifndef LAB3_H
#define	LAB3_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define SEMKEY 3333
#define	SHMKEY 4444
#define TEXT_SIZE 20
#define	NUM	10			// 缓冲区数量

// P、V操作的函数
void P(int semid, int index);
void V(int semid, int index);
// 信号量
int semid;
// 共享内存组
struct shm_st { 
	int num;		// 标记是否结束
	char text[TEXT_SIZE]; //记录写入的文本  
};

union semun {
	int   val;
	struct   semid_ds  *buf;
	unsigned short   *array;
};

/* struct shmid_ds {  
	uid_t shm_perm.uid;  
	uid_t shm_perm.gid;  
	mode_t shm_perm.mode;  
}; */


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

#endif
