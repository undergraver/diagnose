#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void smash(int count)
{
	char buffer[20];
	printf("Writing %d bytes into a stack buffer of %ld\n",count,sizeof(buffer));
	memset(buffer,0,count);
}

int main(int argc, char *argv[])
{
	int count = 10;
	if (argc > 1) {
		count = atoi(argv[1]);
	}
	smash(count);
	return 0;
}
