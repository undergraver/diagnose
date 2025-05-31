#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int func1(long param)
{
	void *ptr = malloc(param);
	printf("Called function with param: %d\n", param);
	return 0;
}

int func0(long param)
{
	printf("before calling func1()\n");
	func1(param);
	printf("after calling func1()\n");
	return 0;
}


int main(int argc, char *argv[])
{
	int wait = 0;
	if (argc > 1) {
		wait = atoi(argv[1]);
	}

	if (wait > 0) {
		printf("Sleeping for %d seconds.\n",wait);
		sleep(wait);
	}


	func0(44);
	return 0;
}
