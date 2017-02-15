#include <setjmp.h>
#include <signal.h>
#include <stdio.h>

jmp_buf buf;

void handler(int s)
{
    if (SIGINT == s)
        puts("Ctrl ^C\n");
    for (;;)
        ;
    longjmp(buf, 1);
}

int main()
{
    signal(SIGINT, handler);
    if (setjmp(buf))
        return 0;
    else
        for (;;)
            ;
    return 0;
}

