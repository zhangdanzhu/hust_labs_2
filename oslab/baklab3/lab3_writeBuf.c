#include "lab3.h"


// 子进程writeBuf负责写
int main(int argc, char *argv[])
{

	int i;
	struct shm_st *buffers[NUM];
	int shmid[NUM];
	semid = semget(SEMKEY, 2, IPC_CREAT|0666);

	// 获取共享内存组
	for (i = 0; i < NUM; i++)
	{
		shmid[i] = shmget((key_t)(i+1000), sizeof(struct shm_st), IPC_CREAT|0666);
		buffers[i] = shmat(shmid[i], NULL, 0);
	}

	// 打开文件
	FILE *tfp = fopen(argv[2], "w+");
	if (NULL == tfp)
	{
		printf("Fail to open target file!\n");
		exit(EXIT_FAILURE);
	}
	
	i = 0;
	while (1)
	{
		P(semid, 1); // 有满的缓冲区才将其写入目标文件
		printf("xiedaowenjian\n");
		if (-1 == buffers[i]->num)
		{// 结束
			return 0;
		}
		fwrite(buffers[i]->text, sizeof(char), buffers[i]->num, tfp);
		i = (i+1) % NUM;
		V(semid, 0);
	}

	fclose(tfp);
	tfp = NULL;
	return 0;
}

