#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

void printdir(char *dir, int depth)
{
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;

	if ((dp = opendir(dir)) == NULL)	// 打开文件失败
	{
		printf("Fail to open dir!");
		return ;
	}
	chdir(dir);

	while ((entry = readdir(dp)) != NULL)	// 读到一个目录项
	{
		lstat(entry->d_name, &statbuf);	// 得到该目录项相关信息 
		if (S_ISDIR(statbuf.st_mode))	// 是目录
		{
			if (!strcmp(entry->d_name, "..") || !strcmp(entry->d_name, "."))
			{	// 跳过目录项名".."或"."的
				continue ;
			}
			// 打印目录项信息
			printf("DIR\t-- DEPTH: %d  NAME: %s\n", depth, entry->d_name);
			// 递归调用printdir，打印子目录信息
			printdir(entry->d_name, depth+4);
		}
		else	// 打印文件相关信息
		{
			printf("FILE\t-- DEPTH: %d  NAME: %s\n", depth, entry->d_name);
		}
	}
	// 返回父目录
	chdir("..");
	// 关闭目录项
	closedir(dp);
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage：\n./printdir DIR_NAME\n");
		return 0;
	}
	printdir(argv[1], 0);
	return 0;
}
