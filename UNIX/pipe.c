#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    char *argvv[2][5] = { { "ls", "|", "wc", "-l", 0}, { "grep", "c", 0 } };
    int status;

    int fd[2];
    pipe(fd);

    if ((pid = vfork()) != -1) {
        if(pid == 0) {        /*Child executing */
            // ls
            close(fd[0]);//close read end
            close(1); // close stdout
            dup(fd[1]); // redirect output to fd[1]
            //close(fd[1]);
            execvp(argvv[0][0], argvv[0]); /* Here's stored the first instruction */

        } else {            /* Parent executing */
            // grep
            wait(&status);
            close(fd[1]); // close read
            close(0); // close stdin
            dup(fd[0]); // redirect input to fd[0]
            //close(fd[0]);
            execvp(argvv[1][0], argvv[1]); /* Here's stored the second instruction */
        }
    }

    return 0;
}