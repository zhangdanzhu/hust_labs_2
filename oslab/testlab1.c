#include "stdio.h"
#include "stdlib.h"
#include "signal.h"
#include "unistd.h"
#include <sys/types.h>
#include <sys/wait.h>

void fsig_deal(int sig); // 父进程信号处理
void p1();               // 子进程1
void p2();               // 子进程2
void SIGURS_deal();     // SIGURS1信号处理
int pid1 = 0, pid2 = 0;
int pipefd[2];

int main(void)
{
	// 创建无名管道
	if(pipe(pipefd) < 0)
	{
		printf("Fail to pipe!\n");
		exit(EXIT_FAILURE);
	}
	// 设置软中断信号SIGINT
	signal(SIGINT, fsig_deal); 
	// 创建子进程1、2
	pid1 = fork();
	if (pid1 < 0)
	{
		printf("fork error!\n");
	}
	else if (pid1 == 0)  // 若是子进程1则执行p1
	{
		p1();
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
			p2();
		}
	}
	// 等待子进程1、2退出
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	printf("Parent Process is Killed!\n");
	// 关闭管道
	close(pipefd[0]);
	close(pipefd[1]);
}

// 父进程信号处理
void fsig_deal(int sig)
{
	// 发SIGUSR1给子进程1
	kill(pid1, SIGUSR1);
	// 发SIGUSR2给子进程2
	kill(pid2, SIGUSR2);
}

// 子进程1
void p1()
{
	int count = 1;
	char buf[50];
	// 设置忽略信号SIGINT
	signal(SIGINT, SIG_IGN);
	// 设置信号SIGUSR1
	signal(SIGUSR1, SIGURS_deal);
	while (1)
	{
		// 发送数据至管道数据
//		close(pipefd[0]);
		sprintf(buf, "I send you %d times.\n", count);
		write(pipefd[1], buf, sizeof(buf));
		read(pipefd[0], buf, sizeof(buf));
		printf("%s", buf);
		// 计数器++
		count++;
		// 睡眠1秒
		sleep(1);
	}
}

// 子进程2
void p2()
{
	char buf[50];
	// 设置忽略信号SIGINT
	signal(SIGINT, SIG_IGN);
	// 设置信号SIGUSR2
	signal(SIGUSR2, SIGURS_deal);
	while (1)
	{
		// 接收管道数据
		close(pipefd[1]);
//		read(pipefd[0], buf, sizeof(buf));
		// 显示数据
//		printf("%s", buf);
	}
}

// SIGURS信号处理
void SIGURS_deal(int sig)
{
	// 关闭管道
	close(pipefd[0]);
	close(pipefd[1]);
	// 显示退出信息
	if (sig == SIGUSR1)
	{
		printf("Child process1 is killed by Parent!\n");
	}
	else if (sig == SIGUSR2)
	{
		printf("Child process2 is killed by Parent!\n");
	}
	// 退出
	exit(EXIT_SUCCESS);
}
