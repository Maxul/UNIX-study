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
#include <fcntl.h>

enum result {
    COMPILE_ERROR,
    RUNTIME_ERROR,
    TIME_LIMIT_EXCEEDED,
    MEMORY_LIMIT_EXCEEDED,
    OUTPUT_LIMIT_EXCEEDED,
    PRESENTATION_ERROR,
    SYSTEM_ERROR,
    WRONG_ANWSER,
    ACCEPTED,
};

#define EXIT_MEG(msg) {printf("%s.\n", msg); _exit(-1);}

int setRlimit()
{
    struct rlimit limit;

    limit.rlim_cur = 1;
    limit.rlim_max = 2;
    if (setrlimit(RLIMIT_CPU, &limit)) {
        printf("%s\n", "Set time failed.");
        return -1;
    }

    limit.rlim_cur = limit.rlim_max = 1024*1024*16;
    if (setrlimit(RLIMIT_AS, &limit)) {
        printf("%s\n", "Set memory failed.");
        return -1;
    }

    return 0;
}

int main()
{
    int ret = system("gcc -o src -Wall ./src.c");

    if (ret != 0)
        EXIT_MEG("Compile Error");

    struct user_regs_struct regs;
    int status;
    char info[256];
    struct rusage ru;

    int fd[2];
    pid_t child;

    child = vfork();

    if (child < 0) {
        EXIT_MEG("vfork failed");
    }

    if(child == 0) {

        if (-1 == setRlimit())
            EXIT_MEG("setrlimit failed");

        if (-1 == ptrace(PTRACE_TRACEME, 0, NULL, NULL))
            EXIT_MEG("ptrace me failed");

        if ((fd[0] = open("./in", O_RDWR|O_CREAT, 0644)) != -1)
            if (-1 != dup2(fd[0], STDIN_FILENO))
                close(fd[0]);

        if ((fd[1] = open("./out", O_RDWR|O_CREAT, 0644)) != -1)
            if (-1 != dup2(fd[1], STDOUT_FILENO))
                close(fd[1]);

        if (-1 == execl("./src", "", NULL))
            _exit(-1);

    } else {

        while(1) {

            if (-1 == wait4(child, &status, WSTOPPED, &ru))
                return 1;

            if(WIFEXITED(status)) {
                break;
            } else if (WSTOPSIG(status) != SIGTRAP) {

                ptrace(PTRACE_KILL, child, NULL, NULL);

                printf("TIME %ldMS MEM%ldKB\n", ru.ru_utime.tv_sec * 1000
                       + ru.ru_utime.tv_usec / 1000
                       + ru.ru_stime.tv_sec * 1000
                       + ru.ru_stime.tv_usec / 1000,
                       ru.ru_maxrss * (sysconf(_SC_PAGESIZE) / 1024));

                switch (WSTOPSIG(status)) {
                case SIGSEGV:
                    printf("%s\n", "Out of Memory");
                    break;
                case SIGALRM:
                case SIGXCPU:
                    printf("%s\n", "Out of Time");
                    break;
                }
            }
            /*
                        if (ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1)
                            return 1;

                        if (regs.orig_rax == SYS_open) {
            //                ptrace(PTRACE_KILL, child, NULL, NULL);
                        }
            */
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);

        }
    }
    return 0;
}
