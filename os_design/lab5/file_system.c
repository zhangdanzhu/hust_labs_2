#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN_MAX 32		// 目录/文件名最大长度
#define PATH_LEN_MAX 512	// 路径名最大长度
#define MAX_CHILD_DIR 8		// 最大子目录数
#define MAX_CHILD_FILE 16	// 最大子文件数
#define DIR_NUM	50		// 最大目录数
#define	FILE_NUM 100	// 最大文件数
#define BLOCK_NUM FILE_NUM	// 磁盘块数


// 目录节点结构
struct dir_node {
	char dir_name[NAME_LEN_MAX];	// 目录名
	int child_dir[MAX_CHILD_DIR];	// 子目录索引
	int dir_count;					// 当前子目录数
	int child_file[MAX_CHILD_FILE];	// 子文件索引
	int file_count;			// 当前子文件数
	int parent;				// 父目录索引
}dir[DIR_NUM];

// 文件节点结构
struct file_node {
	char file_name[NAME_LEN_MAX];	// 文件名
	int file_len;			// 文件长度
	int parent;				// 父目录索引
	int block_index;		// 磁盘块索引
}file[FILE_NUM];

// 磁盘块
struct block {
	char buf[1024];
}block[BLOCK_NUM];

// 目录、文件占用标志位(0闲1占)
int dir_occ[DIR_NUM];
int file_occ[FILE_NUM];
int block_occ[BLOCK_NUM];

// 当前目录
int cur_dir;		

int create_file(int parent, char *file_name);
int get_index(int type);
int search(int parent, char *name, int type);
int make_dir(int parent, char *dir_name);
void show_path(int index);
int del_file(int parent, char *file_name);
int del_dir(int parent, char *dir_name);
int change_dir(char *path);
void list_file(int index, int level);
void list_dir(int index, int level);
void list(int index);
int get_dir(char *path);

/**
* @brief	读文件
* @arg		parent：int型，父目录索引
* @arg		file_name: char *,文件名
* @retval	int
* @return	如果成功则返回文件索引号，否则返回-1
*/
int read_file(int parent, char *file_name)
{
	int index;

	// 按名查找文件
	if (-1 == (index = search(parent, file_name, 1)))	// 文件不存在
	{
		printf("fail to read file '%s' : file doesn't exist!\n", file_name);
		return -1;
	}
	// 读磁盘内容并打印
	printf("%s\n", block[file[index].block_index].buf);
	return index;
}

/**
* @brief	写文件
* @arg		parent：int型，父目录索引
* @arg		file_name: char *,文件名
* @arg		text:	char *,写入内容
* @retval	int
* @return	如果成功则返回文件索引号，否则返回-1
*/
int write_file(int parent, char *file_name, char *text)
{
	int index;

	// 按名查找文件
	if (-1 == (index = search(parent, file_name, 1)))	// 文件不存在
	{
		printf("fail to write file '%s' : file doesn't exist!\n", file_name);
		return -1;
	}

	// 写入
	strcat(block[file[index].block_index].buf, text);
	file[index].file_len += strlen(text);
	printf("write successfully!\n");
	return index;

}

/**
* @brief	创建文件
* @arg		parent：int型，父目录索引
* @arg		file_name: char *,文件名
* @retval	int
* @return	如果成功则返回文件索引号，否则返回-1
*/
int create_file(int parent, char *file_name)
{
	int index;
	int b_index;
	if (dir[parent].file_count == MAX_CHILD_FILE)	// 父目录已满
	{
		printf("fail to create file '%s' : parent dir is full!\n", file_name);
		return -1;
	}
	else if (search(parent, file_name, 1) != -1)	// 同名文件存在
	{
		printf("fail to create file '%s' : file already exists!\n", file_name);
		return -1;
	}

	if (-1 == (index = get_index(1)))	// 总文件数超出
	{
		printf("fail to create file '%s' : file is too much!\n", file_name);
		return -1;
	}
	if (-1 == (b_index = get_index(2))) // 磁盘块不够
	{
		printf("fail to create file '%s' : blocks are not enough !\n", file_name);
		return -1;
	}
	
	// 创建文件
	strcpy(file[index].file_name, file_name);
	file[index].parent = parent;
	file[index].file_len = 0;

	file[index].block_index = b_index;

	dir[parent].child_file[dir[parent].file_count++] = index;

	return index;
}

/**
* @brief	获得索引号
* @arg		type：int型，查找类型（0：目录，1：文件, 2:磁盘块）
* @retval	int
* @return	如果成功则返回索引号，否则返回-1
*/
int get_index(int type)
{
	int i = 0;
	if (1 == type) // 文件
	{
		for (i = 0; i < FILE_NUM; i++)
		{
			if (0 == file_occ[i])
			{
				file_occ[i] = 1;
				return i;
			}
		}
		return -1;
	}
	else if (0 == type) // 目录
	{
		for (i = 1; i < DIR_NUM; i++)
		{
			if (0 == dir_occ[i])
			{
				dir_occ[i] = 1;
				return i;
			}
		}
		return -1; 
	}
	else 
	{
		for (i = 0; i < BLOCK_NUM; i++)
		{
			if (0 == block_occ[i])
			{
				block_occ[i] = 1;
				return i;
			}
		}
		return -1; 
	}

}

/**
* @brief	获得索引号
* @arg		parent:	int型，父目录索引号
* @arg		name: char *型，名
* @arg		type：int型，查找类型（0：目录，1：文件）
* @retval	int
* @return	如果成功则返回索引号，否则返回-1
*/
int search(int parent, char *name, int type)
{
	int i = 0;
	if (type) // 文件
	{
		for (i = 0; i < dir[parent].file_count; i++)
		{
			if (!strcmp(file[dir[parent].child_file[i]].file_name, name))
			{
				return dir[parent].child_file[i];
			}
		}
		return -1;
	}
	else
	{
		for (i = 0; i < dir[parent].dir_count; i++)
		{
			if (!strcmp(dir[dir[parent].child_dir[i]].dir_name, name))
			{
				return dir[parent].child_dir[i];
			}
		}
		return -1; 
	}
}

/**
* @brief	创建目录
* @arg		parent：int型，父目录索引
* @arg		dir_name: char *,目录名
* @retval	int
* @return	如果成功则返回目录索引号，否则返回-1
*/
int make_dir(int parent, char *dir_name)
{
	int index;
	if (dir[parent].dir_count == MAX_CHILD_DIR)	// 父目录已满
	{
		printf("fail to create dir '%s' : parent dir is full!\n", dir_name);
		return -1;
	}
	else if (search(parent, dir_name, 0) != -1)	// 同名目录存在
	{
		printf("fail to create dir '%s' : dir already exists!\n", dir_name);
		return -1;
	}

	if (-1 == (index = get_index(0)))	// 总目录数超出
	{
		printf("fail to create dir '%s' : dir is too much!\n", dir_name);
		return -1;
	}

	// 创建目录
	strcpy(dir[index].dir_name, dir_name);
	dir[index].dir_count = 0;
	dir[index].file_count = 0;
	dir[index].parent = parent;

	dir[parent].child_dir[dir[parent].dir_count++] = index;
	return index;
}

/**
* @brief	显示当前路径
* @arg		index: 当前目录
* @retval	void
*/
void show_path(int index)
{
	if (0 == index)
	{
		printf("/");
	}
	else
	{
		show_path(dir[index].parent);
		printf("%s/", dir[index].dir_name);
	}
}

/**
* @brief	删除文件
* @arg		parent：int型，父目录索引
* @arg		file_name: char *,文件名
* @retval	int
* @return	如果成功则返回文件索引号，否则返回-1
*/
int del_file(int parent, char *file_name)
{
	int index;
	int i;
	// 确认文件是否存在
	if (-1 == (index = search(parent, file_name, 1)))	// 文件不存在
	{
		printf("fail to delete file '%s' : file doesn't exist!\n", file_name);
		return -1;
	}
	// 修改占用标志位
	file_occ[index] = 0;
	// 修改父目录结点
	for (i = 0; i < dir[parent].file_count; i++)
	{
		if (dir[parent].child_file[i] == index)
		{
			for ( ; i < dir[parent].file_count-1; i++)
			{
				dir[parent].child_file[i] = dir[parent].child_file[i+1]; 
			}
			break;
		}
	}
	dir[parent].file_count--;
	// 释放磁盘
	block_occ[file[index].block_index] = 0;
	strcpy(block[file[index].block_index].buf, "\0");
	file[index].file_len = 0;

	return index;
}

/**
* @brief	删除目录
* @arg		parent：int型，父目录索引
* @arg		dir_name: char *,目录名
* @retval	int
* @return	如果成功则	返回目录索引号，否则返回-1
*/
int del_dir(int parent, char *dir_name)
{
	int index;
	int i = 0;
	// 确认目录是否存在
	if (-1 == (index = search(parent, dir_name, 0)))	// 目录不存在
	{
		printf("fail to delete dir '%s' : dir doesn't exist!\n", dir_name);
		return -1;
	}
	// 修改占用标志位
	dir_occ[index] = 0;
	printf("***%d\n", index);
	// 修改父目录结点
	for (i = 0; i < dir[parent].dir_count; i++)
	{
		if (dir[parent].child_dir[i] == index)
		{
		printf("***%d\n", index);
			for ( ; i < dir[parent].dir_count-1; i++)
			{
				dir[parent].child_dir[i] = dir[parent].child_dir[i+1]; 
			}
			break;
		}
	}
	dir[parent].dir_count--;

	return index;
}

/**
* @brief	由路径获取目录索引
* @arg		path：char *型，路径
* @retval	int
* @return	成功则返回目录索引，否则返回-1
*/
int get_dir(char *path)
{
	int i = 0;
	int temp_dir = cur_dir;
	if (path[0] == '/')	// 绝对路径
	{
		temp_dir = 0;
		path++;
	}
	while (1)
	{
		if ('/' == path[i])
		{
			path[i] = '\0';
			if (-1 == (temp_dir = search(temp_dir, path, 0)))
			{
				return -1;
			}
			i++;
			path += i;
			i = 0;
		}
		else if ('\0' == path[i])
		{
			return search(temp_dir, path, 0);
		}
		else
		{
			i++;
		}
	}
	return temp_dir;
}

/**
* @brief	改变目录
* @arg		path：char *型，路径
* @retval	int
* @return	成功则返回目录索引，否则返回-1
*/
int change_dir(char *path)
{
	int temp_dir;
	if (!strcmp(path, "/"))
	{
		temp_dir = 0;
	}
	else if (!strcmp(path, ".."))
	{
		if (cur_dir == 0)
		{
			temp_dir = -1;
		}
		else
		{
			temp_dir = dir[cur_dir].parent;
		}
	}
	else if (!strcmp(path, "."))
	{
		temp_dir = cur_dir;
	}
	else
	{
		temp_dir = get_dir(path);
	}
		
	if (-1 == temp_dir)
	{
		printf("dir not exist!\n");
		return -1;
	}
	else
	{
		return (cur_dir = temp_dir);
	}

}

/**
* @brief	显示文件
* @arg		index：int型，当前目录索引
* @arg		level: int型，当前层数
* @retval	void
*/
void list_file(int index, int level)
{
	int i = 0;
	int l = level;
	for (; l > 0; l--)
	{
		printf("  ");
	}
	printf("<file>  name: %s\tlen: %d\n", file[index].file_name, file[index].file_len);
}

/**
* @brief	显示目录
* @arg		index：int型，当前文件索引
* @arg		level: int型，当前层数
* @retval	void
*/
void list_dir(int index, int level)
{
	int i = 0;
	int l = level;
	for (; l > 0; l--)
	{
		printf("  ");
	}
	printf("<dir>   name: %s\n", dir[index].dir_name);
	for (i = 0; i < dir[index].file_count; i++)
	{
		list_file(dir[index].child_file[i], level+4);
	}
	for (i = 0; i < dir[index].dir_count; i++)
	{
		list_dir(dir[index].child_dir[i], level+4);
	}
}

/**
* @brief	显示当前目录
* @arg		index：int型，当前目录索引
* @retval	void
*/
void list(int index)
{
	int i = 0;
	for (i = 0; i < dir[index].file_count; i++)
	{
		list_file(dir[index].child_file[i], 0);
	}
	for (i = 0; i < dir[index].dir_count; i++)
	{
		list_dir(dir[index].child_dir[i], 0);
	}

	// 统计文件和目录数
	printf("%d files\n", dir[index].file_count);
	printf("%d dirs\n", dir[index].dir_count);
}

int main(int argc, char **argv)
{
	char command[10];
	char name_in[PATH_LEN_MAX];
	char text[1024];
	while (1)
	{
		show_path(cur_dir);
		scanf("%s", command);
		if (!strcmp(command, "mkdir"))
		{
			scanf("%s", name_in);
			if (-1 != make_dir(cur_dir, name_in))
			{
				printf("create dir '%s' successfully!\n", name_in);
			}
		}
		else if (!strcmp(command, "rmdir"))
		{
			scanf("%s", name_in);
			if (-1 != del_dir(cur_dir, name_in))
			{
				printf("delete dir '%s' successfully!\n", name_in);
			}
		}
		else if (!strcmp(command, "vim"))
		{
			scanf("%s", name_in);
			if (-1 != create_file(cur_dir, name_in))
			{
				printf("create file '%s' successfully!\n", name_in);
			}
		}
		else if (!strcmp(command, "rm"))
		{
			scanf("%s", name_in);
			if (-1 != del_file(cur_dir, name_in))
			{
				printf("delete file '%s' successfully!\n", name_in);
			}

		}
		else if (!strcmp(command, "cat"))
		{
			scanf("%s", name_in);
			read_file(cur_dir, name_in);
		}
		else if (!strcmp(command, "echo"))
		{
			scanf("%s", name_in);
			getchar();
			gets(text);
			write_file(cur_dir, name_in, text);
		}
		else if (!strcmp(command, "cd"))
		{
			scanf("%s", name_in);
			change_dir(name_in);
		}
		else if (!strcmp(command, "ls"))
		{
			list(cur_dir);
		}
		else if (!strcmp(command, "exit"))
		{
			break;
		}
		else
		{
			printf("wrong command!\n");
		}
	}

	return 0;
}
