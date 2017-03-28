#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
	int dp;
	char buf[] = "This is 1 line\n";
	char buf2[] = "this is 2 line\n";
	char buf3[] = "this is 3 line\n";
	char r_buf[16];
	dp = open("/dev/my_dev", O_RDWR);
	write(dp, buf, 16);
	write(dp, buf2, 16);
	write(dp, buf3, 16);
	close(dp);
	dp = open("/dev/my_dev", O_RDWR);
	read(dp, r_buf, 16);
	printf("%s", r_buf);
	read(dp, r_buf, 16);
	printf("%s", r_buf);
	read(dp, r_buf, 16);
	printf("%s", r_buf);
	close(dp);
	return 0;

}

