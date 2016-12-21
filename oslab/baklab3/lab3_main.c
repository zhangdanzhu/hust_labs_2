#include "lab3.h"

int main(int argc, char *argv[])
{
	int pid1, pid2;
	union semun  sem_args;
	unsigned short array[2]={NUM,0};
	sem_args.array = array;
	
	// 创建共享内存组
	int i;
	int shmid[NUM];
	for (i = 0; i < NUM; i++)
	{
		shmid[i] = shmget((key_t)(i+1000), sizeof(struct shm_st), IPC_CREAT|0666);
	}

	// 创建信号灯
	semid = semget(SEMKEY, 2, IPC_CREAT|0666);
	// 信号灯赋初值
	semctl(semid, 1, SETALL, sem_args);
	// 创建两进程
	pid1 = fork();
	if (pid1 < 0)
	{
		printf("fork error!\n");
	}
	else if (pid1 == 0)  // 若是子进程1则执行p1
	{
		execv("./lab3_readBuf", argv);
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
			execv("./lab3_writeBuf", argv);
		}
	}
	// 等待两个进程运行结束
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	// 删除信号灯
	semctl(semid, 2, IPC_RMID);
	// 删除共享内存组
	for (i = 0; i < NUM; i++)
	{
		shmctl(shmid[i], IPC_RMID, NULL);
	}
	return 0;
}
