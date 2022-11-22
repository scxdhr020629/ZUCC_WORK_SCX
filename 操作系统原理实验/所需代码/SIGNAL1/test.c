// #include <unistd.h>
// #include <signal.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <stdlib.h>
// #include <stdio.h>
// int f1, f2, f3;
// int p1, p2;
// void waiting1()
// {
//     while (f1 == 0)
//         ;
// }
// void waiting2()
// {
//     while (f2 == 0)
//         ;
// }
// void waiting3()
// {
//     while (f3 == 0)
//         ;
// }
// void hander1()
// {
//     (void)signal(SIGINT, SIG_DFL);
//     f1 = 1;
//     kill(p1, 16);
// }
// void hander2()
// {
//     f2 = 1;
//     kill(p2, 17);
//     printf("Child process is killed by parent!\n");
// }
// void hander3()
// {
//     f3 = 1;
//     printf("Grandson process1 is killed by son!\n");
// }
// int main()
// {

//     (void)signal(SIGINT, hander1);
//     while ((p1 = fork()) == -1)
//         ;
//     if (p1 > 0) // yeye
//     {
//         //(void)signal(SIGINT, hander1);
//         waiting1();
//         wait(0);
//         wait(0);
//         printf("Parent process is killed!\n");
//     }
//     else
//     { // erzi
//         while ((p2 = fork()) == -1)
//             ;
//         if (p2 > 0) // erzi
//         {
//             (void)signal(16, hander2);
//             waiting2();
//             wait(0);
//             exit(0);
//         }
//         else // sunzi
//         {
//             (void)signal(17, hander3);
//             waiting3();
//             exit(0);
//         }
//     }
// }

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
    else /*p1进程的处理*/
    {
        while ((p2 = fork()) == -1)
            ;

        if (p2 > 0) // p1进程处理
        {
            wait_mark = 1;
            signal(16, stop);
            waiting(); /*等待信号16*/
            sleep(1);
            /*用上锁的方法实现互斥*/
            lockf(stdout, 1, 0);
            printf("Child process is killed by parent! \n");
            lockf(stdout, 0, 0);
            /*模拟P1被kill时进程的工作*/
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
            printf("Grandson process1 is killed by son! \n");
            lockf(stdout, 0, 0);
            /*模拟P2被kill时进程的工作*/
            exit(0);
        }

        //
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