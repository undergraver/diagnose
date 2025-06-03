#include <stdlib.h>

void init_lut();

int initialized = 0;
int bit_count[256];

int countSetBits(unsigned char n)
{
    if (initialized == 0)
    {
	    init_lut();
	    initialized = 1;
    }
    return bit_count[n];
}

int countSetBitsOld(unsigned char n) {
    int count = 0;
    while (n) {
        count += n & 1;  // Check if the least significant bit is set
        n >>= 1;         // Shift right to process the next bit
    }
    return count;
}



void init_lut()
{
	for(int i=0;i<256;i++)
	{
		bit_count[i] = countSetBitsOld((char)i);
	}
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
