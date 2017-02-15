/**
	Warning:
	This might crash the system!!!

**/


#include <stdio.h>
#include <stdlib.h>

int main()
{
    size_t block_size[] = { 1024*1024, 1024, 1 };
    static size_t maximum;
    int i, count;
    /*
    	for (i = 0; i < sizeof block_size; ++i) {
    		for (count = 1; ; ++ count) {
    			void *block = malloc(maximum + block_size[i] * count);
    			if (block) {
    				maximum += block_size[i] * count;
    				free(block);
    			} else {
    				break;
    			}
    		}
    	}
    */
    while (malloc(1 << 20))
        maximum += 1;
    printf("%lu\n", maximum);
    return 0;
}

