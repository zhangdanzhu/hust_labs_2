#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

int main()
{
	syscall(326, "test0.c", "test1.c");
	return 0;
}
