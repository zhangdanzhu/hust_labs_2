#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

void printright(mode_t mode)
{
	// 用户权限
	mode_t temp;
	if (((temp = mode) & S_IRUSR) == S_IRUSR)
	{
		printf("r");
	}
	else
	{
		printf("-");
	}
	if (((temp = mode) & S_IWUSR) == S_IWUSR)
	{
		printf("w");
	}
	else
	{
		printf("-");
	}
	if (((temp = mode) & S_IXUSR) == S_IXUSR)
	{
		printf("x");
	}
	else
	{
		printf("-");
	}
	// 组权限
	if (((temp = mode) & S_IRGRP) == S_IRGRP)
	{
		printf("r");
	}
	else
	{
		printf("-");
	}
	if (((temp = mode) & S_IWGRP) == S_IWGRP)
	{
		printf("w");
	}
	else
	{
		printf("-");
	}
	if (((temp = mode) & S_IXGRP) == S_IXGRP)
	{
		printf("x");
	}
	else
	{
		printf("-");
	}
	// 其他
	if (((temp = mode) & S_IROTH) == S_IROTH)
	{
		printf("r");
	}
	else
	{
		printf("-");
	}
	if (((temp = mode) & S_IWOTH) == S_IWOTH)
	{
		printf("w");
	}
	else
	{
		printf("-");
	}
	if (((temp = mode) & S_IXOTH) == S_IXOTH)
	{
		printf("x");
	}
	else
	{
		printf("-");
	}

	printf("\t");
}

void printdir(char *dir, int depth)
{
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	int i;
	struct passwd *u_info;
	struct group *g_info;
	char *timebuf;
	if ((dp = opendir(dir)) == NULL)	// 打开目录失败
	{
		printf("Fail to open dir!");
		return ;
	}
	chdir(dir);

	while ((entry = readdir(dp)) != NULL)	// 读到一个目录项
	{	
		lstat(entry->d_name, &statbuf);	// 得到该目录项相关信息 
		u_info = getpwuid(statbuf.st_uid);
		g_info = getgrgid(statbuf.st_gid);
		if (S_ISDIR(statbuf.st_mode))	// 是目录
		{
			if (!strcmp(entry->d_name, "..") || !strcmp(entry->d_name, "."))
			{	// 跳过目录项名".."或"."的
				continue ;
			}
			for (i = 0; i < depth; i++) // 打印层数
			{
				printf(" ");
			}
			// 打印目录项信息
			printright(statbuf.st_mode);	// 打印权限
			timebuf = ctime(&statbuf.st_ctime);	// 获取创建时间
			timebuf[strlen(timebuf)-1] = '\0';	// 去掉‘\n’
			printf("%s\t%s\t%ld\t%s\t%s\n", u_info->pw_name, g_info->gr_name, statbuf.st_size, timebuf, entry->d_name);
			// 递归调用printdir，打印子目录信息
			printdir(entry->d_name, depth+4);
		}
		else	// 打印文件相关信息
		{
			for (i = 0; i < depth; i++) // 打印层数
			{
				printf(" ");
			}
			printright(statbuf.st_mode);	// 打印权限
			timebuf = ctime(&statbuf.st_ctime);
			timebuf[strlen(timebuf)-1] = '\0';	// 去掉‘\n’
			printf("%s\t%s\t%ld\t%s\t%s\n", u_info->pw_name, g_info->gr_name, statbuf.st_size, timebuf, entry->d_name);
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
