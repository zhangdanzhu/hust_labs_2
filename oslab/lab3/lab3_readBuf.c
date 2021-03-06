#include "lab3.h"


// 子进程readBuf负责从缓冲区读数据到文件
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
		if (!buffers[i].num)
		{// 结束
			printf("finish copying!\n");
			return 0;
		}
		// 未结束则写入文件同时移到下一个
		fwrite(buffers[i].text, sizeof(char), buffers[i].num, tfp);
		i = (i+1) % NUM;
		V(semid, 0);
	}

	fclose(tfp);
	tfp = NULL;
	return 0;
}

