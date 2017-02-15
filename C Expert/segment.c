#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int x;
int main(int ac, char **av)
{
    extern char etext, edata, end;

    printf("pid: %d\n", getpid());
    printf("code: %p\ndata: %p\nbss: %p\n", &etext, &edata, &end);
    sbrk(0x15);
    printf("brk: %p\n", sbrk(0));

    printf("malloc: %p\n", malloc(20));
    printf("global: %p\n", &x);

    int xx;
    printf("local: %p\n", &xx);

    printf("env: %p\n", &ac);

    char s[128];
    sprintf(s, "cat /proc/%d/maps", getpid());
    system(s);

    return 0;
}
