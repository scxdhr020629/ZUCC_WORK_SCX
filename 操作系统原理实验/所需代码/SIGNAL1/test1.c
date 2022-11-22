#include <stdio.h>
#include <signal.h>
#include <unistd.h>
int i = 1;
void hander1()
{
    printf("child caught signal \n");
}
void hander2()
{
    printf("parent caught signal \n");
}

void main()
{
    int p1, p2;

    while ((p2 = fork()) == -1)
        ;
    if (p2 > 0) //主进程
    {
        signal(17, hander1);
        while (1)
        {
            pause();
            kill(p2, 16);
        }
    }
    else
    {
        signal(16, hander2);
        while (1)
        {

            p1 = getppid();
            kill(p1, 17);
            pause();
        }
    }
}