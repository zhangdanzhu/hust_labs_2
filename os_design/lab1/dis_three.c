#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
	int pid1,pid2,pid3;
	if ((pid1 = fork()) < 0)
	{	
		printf("fork 1 error!\n");
	}
	else if (0 == pid1)
	{
		execv("./dis_time", argv);
	}
	else
	{
		if ((pid2 = fork()) < 0)
		{
			printf("fork 2 error!\n");
		}
		else if (0 == pid2)
		{
			execv("./dis_cpu", argv);
		}
		else
		{
			if ((pid3 = fork()) < 0)
			{
				printf("fork 3 error!\n");
			}
			else if (0 == pid3)
			{
				execv("./dis_sum", argv);
			}
			else
			{
				waitpid(pid1,NULL,0);
				waitpid(pid2,NULL,0);
				waitpid(pid3,NULL,0);
			}
		}
	}
	return 0;
}
