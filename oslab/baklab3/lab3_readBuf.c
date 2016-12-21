#include "lab3.h"


// 子进程readBuf负责读
int main(int argc, char *argv[])
{

	int i;
	struct shm_st *buffers[NUM];
	int shmid[NUM];
	int finish = 0;
	semid = semget(SEMKEY, 2, IPC_CREAT|0666);

	// 获取共享内存组
	for (i = 0; i < NUM; i++)
	{
		shmid[i] = shmget((key_t)(i+1000), sizeof(struct shm_st), IPC_CREAT|0666);
		buffers[i] = shmat(shmid[i], NULL, 0);
	}

	// 打开文件
	FILE *sfp = fopen(argv[1], "r");
	if (NULL == sfp)
	{
		printf("Fail to open source file!\n");
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (!finish)
	{
		P(semid, 0); // 有空的缓冲区才读入
		printf("dudaobuf\n");
		if ((buffers[i]->num = fread(buffers[i]->text, sizeof(char), TEXT_SIZE, sfp)) != 0)		// 未读完
		{
			i = (i + 1) % NUM;
			V(semid, 1);
		}
		else
		{
			finish = 1;
			buffers[i]->num = -1;
			V(semid, 1);
		}
	}

	fclose(sfp);
	sfp = NULL;
	return 0;
}


