#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
int wait_mark;
void waiting(), stop();
void op(int signum, siginfo_t *info, void *myact)
{
    printf("%s\n", (*info).si_ptr);
}
int main(int argc, char **argv)
{
    struct sigaction act;
    union sigval mysigval_father, mysigval_son, mysigval_grandson;

    int sig = atoi(argv[1]);
    int p1, p2;
    //数据输入
    char data_father[40] = "father process is killed";
    mysigval_father.sival_ptr = data_father;
    char data_son[40] = "child process is killed by father";
    mysigval_son.sival_ptr = data_son;
    char data_grandson[50] = "Grandson process is killed by son";
    mysigval_grandson.sival_ptr = data_grandson;

    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = op;

    if (sigaction(sig, &act, NULL) < 0)
    {
        printf("install sigal error\n");
    }
    while ((p1 = fork()) == -1)
        ;
    if (p1 > 0) // father
    {
        wait(0);
        sigqueue(getpid(), sig, mysigval_father);

        exit(0);
    }
    else // son
    {
        while ((p2 = fork()) == -1)
            ;
        if (p2 > 0) // son
        {
            wait(0);
            sigqueue(getpid(), sig, mysigval_son);

            exit(0);
        }
        else // grandson
        {
            sigqueue(getpid(), sig, mysigval_grandson);
        }
    }
}
