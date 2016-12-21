#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define MYKEY 1111
#define TEXT_SIZE 20

// P、V操作的函数
void P(int semid, int index);
void V(int semid, int index);
// 信号量、累加中间结果、线程句柄
int semid;
int sum = 0;
// 共享内存组
struct shm_st {  
	char text[TEXT_SIZE]; //记录写入的文本  
};

struct shm_st *buffers[10];

// 读、写进程
void readBuf(char *);
void writeBuf(char *);

union semun {
	int   val;
	struct   semid_ds  *buf;
	unsigned short   *array;
};

struct shmid_ds {  
	uid_t shm_perm.uid;  
	uid_t shm_perm.gid;  
	mode_t shm_perm.mode;  
}; 

// 子进程readBuf负责读
void readBuf(FILE *sfp)
{
	int i = 0;
	while ()
	{
		P(semid, 0); // 有空的缓冲区才读入
		if (fgets(buffers[i], TEXT_SIZE, sfp) != NULL)
		{
			// 未读完则移到下一个存储区
			i = (i + 1) % 10;
			V(semid, 1);
		}
		else // 已读完
		{
			
		}
	}

	fclose(sfp);
	sfp = NULL;
	return ;
}

// 子进程writeBuf负责写
void writeBuf(char *target_file_name)
{
	int i = 0;
	P(semid, 1); // 有满的缓冲区才将其写入目标文件

	V(semid, 0);

	fclose(tfp);
	tfp = NULL;
	return ;
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

int main(int argc, char *argv[])
{
	union semun  sem_args;
	unsigned short array[2]={10,0};
	sem_args.array = array;

	// 打开文件
	FILE *sfp = fopen(argv[1], "r");
	FILE *tfp = fopen(argv[2], "w+");
	if (NULL == sfp)
	{
		printf("Fail to open source file!\n");
		exit(EXIT_FAILURE);
	}
	if (NULL == tfp)
	{	
		printf("Fail to open target file!\n");
		exit(EXIT_FAILURE);
	}

	// 创建共享内存组
	int i;
	int shmid[10];
	for (i = 0; i < 10; i++)
	{
		shmid[i] = shmget((key_t)(i+1000), sizeof(struct shm_st), IPC_CREAT|0666);
		buffers[i] = shmat(shmid[i], NULL, 0);
	}

	// 创建信号灯
	semid = semget(MYKEY, 2, IPC_CREAT|0666);
	// 信号灯赋初值
	semctrl(semid, 1, SETALL, sem_srgs);
	// 创建两进程
	pid1 = fork();
	if (pid1 < 0)
	{
		printf("fork error!\n");
	}
	else if (pid1 == 0)  // 若是子进程1则执行p1
	{
		readBUf(source_file_name);
	}
	else
	{
		pid2 = fork(); // 若是父进程则创建子进程2
		if (pid2 < 0)
		{
			printf("fork error!\n");
		}
		else if (pid2 == 0) // 若是子进程2则执行p2
		{
			writeBuf(target_file_name);
		}
	}
	// 等待两个进程运行结束
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	// 删除信号灯
	semctl(semid, 2, IPC_RMID);
	// 删除共享内存组
	return 0;
}
