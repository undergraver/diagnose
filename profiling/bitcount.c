#include <stdlib.h>

int countSetBits(unsigned char n)
{
    int count = 0;
    while (n) {
        count += n & 1;  // Check if the least significant bit is set
        n >>= 1;         // Shift right to process the next bit
    }
    return count;
}


int main(int argc,char *argv[])
{
	int count = 5000;
	if(argc>1)
	{
		count = atoi(argv[1]);
	}
	for(int i=0;i<count;i++)
	{
		unsigned char x = rand() % 256;
		countSetBits(x);
	}
	return 0;
}
