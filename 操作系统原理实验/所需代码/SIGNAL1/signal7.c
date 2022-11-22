#include <stdio.h>
#include <signal.h>
#include <unistd.h>
int wait_mark;
void waiting(), stop();

void main()
{
    int p1, p2;

    signal(SIGINT, stop);
    while ((p1 = fork()) == -1)
        ;
    if (p1 > 0) /*主进程的处理*/
    {
        while ((p2 = fork()) == -1)
            ;
        /*主进程的处理*/
        if (p2 > 0)
        {
            wait_mark = 1;
            /*等待接收ctrl+c信号*/
            waiting();
            /*向p1发出信号16*/
            kill(p1, 16);
            /*向p2发出信号17*/
            kill(p2, 17);
            /*同步*/
            wait(0);
            wait(0);
            printf("parents is killed \n");
            exit(0);
        }
        else /*p2进程的处理*/
        {
            wait_mark = 1;
            signal(17, stop);
            waiting(); /*等待信号17*/
            sleep(1);
            /*用上锁的方法实现互斥*/
            lockf(stdout, 1, 0);
            printf("P2 is killed by parent \n");
            lockf(stdout, 0, 0);
            /*模拟P2被kill时进程的工作*/
            exit(0);
        }
    }
    else /*p1进程的处理*/
    {
        wait_mark = 1;
        signal(16, stop);
        waiting(); /*等待信号16*/
        sleep(1);
        /*用上锁的方法实现互斥*/
        lockf(stdout, 1, 0);
        printf("P1 is killed by parent \n");
        lockf(stdout, 0, 0);
        /*模拟P1被kill时进程的工作*/
        exit(0);
    }
}

void waiting()
{
    while (wait_mark != 0)
        ;
}

void stop()
{
    wait_mark = 0;
}
