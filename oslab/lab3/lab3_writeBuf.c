#include "lab3.h"


// 子进程writeBuf负责写入缓冲区
int main(int argc, char *argv[])
{

	int i;
	struct shm_st *buffers;
	int shmid;
	semid = semget(SEMKEY, 2, IPC_CREAT|0666);

	// 获取共享内存组
	shmid = shmget(SHMKEY, sizeof(struct shm_st) * NUM, IPC_CREAT|0666);
	buffers = shmat(shmid, NULL, 0);

	// 打开文件
	FILE *sfp = fopen(argv[1], "r");
	if (NULL == sfp)
	{
		printf("Fail to open source file!\n");
		buffers[0].num = 0;
		V(semid, 1);
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (1)
	{
		P(semid, 0); // 有空的缓冲区才读入
		if ((buffers[i].num = fread(buffers[i].text, sizeof(char), TEXT_SIZE, sfp)) != 0)		// 未读完
		{
			i = (i + 1) % NUM;
			V(semid, 1);
		}
		else
		{
			V(semid, 1);
			break ;
		}
	}

	fclose(sfp);
	sfp = NULL;
	return 0;
}


