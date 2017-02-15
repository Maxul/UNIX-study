#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int
main(int argc, char *argv[])
{
    int pipefd[2];
    pid_t cpid;
    char buf;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {    /* Child reads from pipe */

        close(pipefd[0]);          /* Close unused read end */
        int oldfd = open("./txt", O_RDWR|O_CREAT, 0644);
        dup2(oldfd, STDOUT_FILENO);
        close(oldfd);
        execl("/bin/ls", "", NULL);
        close(pipefd[1]);          /* Reader will see EOF */
        _exit(EXIT_SUCCESS);

    } else {            /* Parent writes argv[1] to pipe */

        close(pipefd[1]);          /* Close unused write end */

        while (read(pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);
        close(pipefd[0]);
        wait(NULL);                /* Wait for child */
        exit(EXIT_SUCCESS);
    }
}