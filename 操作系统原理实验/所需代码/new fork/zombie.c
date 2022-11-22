
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main()
{
    int pid;
    pid = fork();
    if (pid < 0)
    {
        perror("fork error:");
        exit(1);
    }
    else if (pid == 0)
    {
        printf("I am the child process.\n");
    }
    else
    {
        printf("I am the father process\n");
    }
    int i = 0;
    for (i = 0; i < 5; i++)
    {
        system("ps -o pid,ppid,state,tty,command");
    }
}