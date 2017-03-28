#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char **argv)
{
	FILE *fp_src, *fp_tar;
	char buf[30];
	int n;
	if (argc != 3)
	{
		printf("Wrong arguments!\nUsage:\n\t./mycopy source_file_name target_file_name\n");
		return -1;
	}
	else if ((fp_src = fopen(argv[1], "r")) == NULL)
	{
		printf("Cannot open source file!\n");
		return -1;
	}
	else if ((fp_tar = fopen(argv[2], "w+")) == NULL)
	{
		printf("Cannot open target file!\n");
		return -1;
	}
	while ((n = fread(buf, 1, 30, fp_src))!= 0)
	{
		fwrite(buf, 1, n, fp_tar);
	}
	return 0;
	
}
