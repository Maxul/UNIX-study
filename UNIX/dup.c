#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    int oldfd[2];

    oldfd[0] = open("./out", O_RDWR|O_CREAT, 0644);
    dup2(oldfd[0], STDOUT_FILENO);
    close(oldfd[0]);

    oldfd[1] = open("./in", O_RDWR|O_CREAT, 0644);
    dup2(oldfd[1], STDIN_FILENO);
    close(oldfd[1]);
    execl("./src", "", NULL);

    return 0;
}
