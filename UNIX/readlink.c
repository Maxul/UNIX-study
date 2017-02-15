/*
	the following example explains
	how to read a specified folder's
	content under Linux
*/

#include <stdio.h>
#include <unistd.h>
#include <libgen.h>

int main(int argc, char *argv[])
{
    char pathname[128] = {0};
    char path[128] = {0};

    readlink("/proc/self/exe", pathname, sizeof pathname);

    printf("%s\n", pathname);
    printf("%s\n", dirname(argv[1]));
    return 0;
}
