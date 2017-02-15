#include <stdlib.h>
#include <string.h>

#define SIZE (1<<30)

#ifdef USING_CACHE
#define CPY memcpy(dest, src, SIZE)
#else
#define CPY \
    for (i = 0; i < SIZE; ++i) \
    	dest[i] = src[i]
#endif

char src[SIZE], dest[SIZE];

int main()
{
    int i;
    CPY;
    return 0;
}

