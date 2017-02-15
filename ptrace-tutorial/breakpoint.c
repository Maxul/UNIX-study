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

int main(int argc, char const *argv[])
{
    pid_t child;
    const int long_size = sizeof(long);
    child = fork();
    if(child == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl("./dum", "dum", NULL);
    }
    else {
        int status;
        union u {
            long val;
            char chars[long_size];
        } data;
        struct user_regs_struct regs;
        int start = 0;
        long ins;
        while(1) {
            wait(&status);
            if(WIFEXITED(status))
                break;
            ptrace(PTRACE_GETREGS, child, NULL, &regs);
            if(start == 1) {
                ins = ptrace(PTRACE_PEEKTEXT, child, regs.rip, NULL);
                printf("rip: %lx Instruction executed: %lx \n", regs.rip, ins);
            }
            if(regs.orig_rax == SYS_write) {
                start = 1;
                ptrace(PTRACE_SINGLESTEP, child, NULL, NULL);
            }
            else
                ptrace(PTRACE_SYSCALL, child, NULL, NULL);
        }
    }
    return 0;
}