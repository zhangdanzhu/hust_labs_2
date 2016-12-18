#include "stdio.h"
#include "stdlib.h"
#include "string.h"

struct bb {
	int a;
	int b;
};

int main(void)
{
	int test;
	struct bb test;
	if (strcmp(NULL, "SDA") != 0)
	{
		printf("ok");
	}
	return 0;
}
