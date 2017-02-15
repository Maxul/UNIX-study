#include <sys/resource.h>
#include <sys/syscall.h>
#include <sys/ptrace.h>
#include <sys/time.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void reverse(char *str)
{
    int i, j;
    char temp;
    for(i = 0, j = strlen(str) - 2; i <= j; ++i, --j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

const int long_size = sizeof(long);
void getdata(pid_t child, long addr, char *str, int len)
{
    char *laddr;
    int i, j;
    union u {
        long val;
        char chars[long_size];
    } data;

    i = 0;
    j = len / long_size;
    laddr = str;
    while(i < j) {
        data.val = ptrace(PTRACE_PEEKDATA, child, addr + i * 4, NULL);
        memcpy(laddr, data.chars, long_size);
        ++i;
        laddr += long_size;
    }
    j = len % long_size;
    if(j != 0) {
        data.val = ptrace(PTRACE_PEEKDATA, child, addr + i * 4, NULL);
        memcpy(laddr, data.chars, j);
    }
    str[len] = '\0';
}

void putdata(pid_t child, long addr, char *str, int len)
{
    char *laddr;
    int i, j;
    union u {
        long val;
        char chars[long_size];
    } data;
    i = 0;
    j = len / long_size;
    laddr = str;
    while(i < j)
    {
        memcpy(data.chars, laddr, long_size);
        ptrace(PTRACE_POKEDATA, child, addr + i * 4, data.val);
        ++i;
        laddr += long_size;
    }

    j = len % long_size;
    if(j != 0)
    {
        memcpy(data.chars, laddr, j);
        ptrace(PTRACE_POKEDATA, child, addr + i * 4, data.val);
    }
}
int main()
{
    struct user_regs_struct regs;
    long int params[3];
    int status;
    char *str, *laddr;

    pid_t child;

    child = fork();

    if(child == 0) {

        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl("/bin/ls", "ls", "-al", NULL);

    } else {

        while(1) {

            if (-1 == wait(&status))
                return -1;
            if(WIFEXITED(status))
                break;

            if (ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1)
                return -1;

            if(regs.orig_rax == SYS_write) {

                params[0] = regs.rbx; // fd
                params[1] = regs.rcx; // address
                params[2] = regs.rdx; // length

                str = (char *)calloc((params[2]+1), sizeof(char));
                getdata(child, params[1], str, params[2]);
                reverse(str);
                putdata(child, params[1], str, params[2]);

            }
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
        }
    }
    return 0;
}
